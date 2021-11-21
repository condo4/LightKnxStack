#include <stdint.h>
#include <tim.h>
#include <main.h>
#include <stm32l432xx.h>
#include "../layer1_physical.h"
#include "../debug.h"

/* TIM2 Counter periode is 1uS */
#define TICK_IN_BIT     104 /* 1 Bit == 104uS */
#define BIT_IN_BYTE     13


#define CURRENT_RX_BIT              (TIM2->CNT / TICK_IN_BIT)
#define CURRENT_TX_BIT              ((TIM2->CNT / TICK_IN_BIT) - _begin_of_tx)
#define CURRENT_BYTE(bit)           (bit / BIT_IN_BYTE)
#define CURRENT_BIT_POSITION(bit)   (bit % BIT_IN_BYTE)


/*
 * DataLen: Lenght of Layer4 Data in a frame,
 * in Standard Frame Format, this is find in the Byte #5
 */
#define STD_FRAME_SIZE_IN_BYTE(DataLen) (8 + DataLen)
#define STD_FRAME_SIZE_IN_TICK(DataLen) ((8 * BIT_IN_BYTE) + (DataLen * BIT_IN_BYTE))


#define STD_FRAME_BYTE_CTRL 0
#define STD_FRAME_BYTE_LEN  5




#define BIT_POSITION_START 0
#define BIT_POSITION_DATA0 1
#define BIT_POSITION_DATA1 2
#define BIT_POSITION_DATA2 3
#define BIT_POSITION_DATA3 4
#define BIT_POSITION_DATA4 5
#define BIT_POSITION_DATA5 6
#define BIT_POSITION_DATA6 7
#define BIT_POSITION_DATA7 8
#define BIT_POSITION_CRC   9
#define BIT_POSITION_STOP1 10
#define BIT_POSITION_STOP2 11
#define BIT_POSITION_STOP3 12

#define ACK_BEGIN 117
#define ACK_END   130

#define DATA_VALUE(position) (1 << (position - 1))

#define STATE_NOT_INIT       0
#define STATE_WAIT_BUS_FREE  1
#define STATE_BUS_FREE       2
#define STATE_RX_IN_PROGRESS 3
#define STATE_ACK_FRAME      4
#define STATE_WAIT_END_FRAME 5
#define STATE_TX_IN_PROGRESS 6

/* State machine */
static volatile uint8_t _state             = STATE_NOT_INIT;

static volatile uint8_t _current_bit_state = 1;
static volatile uint8_t _tx_bit_val    = 0;
static volatile uint8_t _need_to_ack       = 0;
static volatile int     _stop_pulse        = 0;
static volatile int     _rsync             = 0;
static volatile int     _tx_priority       = Invalid;
static volatile uint8_t *_tx = NULL;
static volatile uint8_t _tx_len = 0;
static volatile int     _begin_of_tx       = 0;


static inline void set_state(uint8_t s)
{
#ifdef DEBUG_STKNX_READ
    if(s == STATE_WAIT_BUS_FREE)
    {
        console_print_char('\r');
        console_print_char('\n');
    }
    console_print_char('{');
    console_print_int(s);
    console_print_char('}');
#endif
    _state = s;
}



void Ph_Knx_Set_Wait_For_Reset(void)
{
#ifdef DEBUG_STKNX_READ
    console_print_char('R');
#endif
    set_state(STATE_WAIT_BUS_FREE);
    _current_bit_state = 1;

    TIM2->CR1 |= TIM_CR1_CEN; /* Enable timer */
    TIM2->CNT = 0; /* Set counter start to 0 */
    TIM2->CCR1 = 50 * TICK_IN_BIT; /* Set Channel 1 Comparator to wait 50 bit times without RX IT */
    TIM2->DIER |= TIM_IT_CC2;  /* Enable interrupt for TIM2 Compare */
}


static inline void tx_pulse(uint32_t bit)
{
    /* Enable Timer Output Tx */
    int start_pulse = (bit * TICK_IN_BIT);
    TIM2->CCR2 = start_pulse;
    TIM2->CCER |= TIM_CCER_CC2E;
    /* 1 pulse must be 35uS */
    _stop_pulse = start_pulse + 35;
}

static inline void ch1_tim_event()
{
    #define ERROR_FRAME  1
    #define ERROR_PARITY 2
    static int error = 0;
    static unsigned int data_len = 1;
    static uint8_t current_rx_byte   = 0;
    static uint8_t current_tx_byte   = 0;

    switch(_state)
    {

        case STATE_WAIT_BUS_FREE:
        {
            /* Bus free detection */
            /* Set Triger to detect first bit */
            _current_bit_state = 1;
            _tx_priority = Invalid;
            set_state(STATE_BUS_FREE);

            /* Disable TIM */
            TIM2->CR1 &= ~TIM_CR1_CEN; /* Disable timer */
            TIM2->CR1 &= ~TIM_CCER_CC1E; /* Disable Compare 1 output */
            TIM2->CNT = 0; /* Set counter start to 0 */
            TIM2->CCR1 = 70; /* Read first bit 70uS after falling edge */
            TIM2->DIER |= TIM_IT_CC1;  /* Enable interrupt for Channel1 Compare */
            TIM2->CCER |= TIM_CCER_CC1E; /* Enable Compare 1 output */
#ifdef DEBUG_STKNX_READ
            console_print_string(">\r\n");
#endif
            break;
        }

        case STATE_RX_IN_PROGRESS:
        {
            int current_bit = CURRENT_RX_BIT;
            TIM2->CCR1 = (TICK_IN_BIT * (current_bit + 1)) + 70; /* Next bit in 104uS */

            if(CURRENT_BYTE(current_bit) < STD_FRAME_SIZE_IN_BYTE(data_len))
            {
                /* Still IN the RX frame */
                if(CURRENT_BIT_POSITION(current_bit) == BIT_POSITION_START)
                {
                    /* First Bit is Start Bit, must be 0 */
                    if(_current_bit_state == 1)
                    {
                        error = ERROR_FRAME;
#ifdef DEBUG_STKNX_READ
                        console_print_char('f');
#endif
                    }
                    else
                    {
                        /* Initialize the error flag for the frame */
                        error = 0;
                    }
                }
                else if(CURRENT_BIT_POSITION(current_bit) >= BIT_POSITION_DATA0 && CURRENT_BIT_POSITION(current_bit) <= BIT_POSITION_DATA7)
                {
                    /* Recived the content of a Byte */
                    if(_current_bit_state == 1)
                    {
                        current_rx_byte |= DATA_VALUE(CURRENT_BIT_POSITION(current_bit));
                    }
                }
                else if(CURRENT_BIT_POSITION(current_bit) == BIT_POSITION_CRC)
                {
                    /* Check parity */
                    uint8_t x = current_rx_byte;
                    x ^= x >> 4;
                    x ^= x >> 2;
                    x ^= x >> 1;
                    x = x & 1;
                    if(x != _current_bit_state)
                    {
                        /* PARITY ERROR */
                        error |= ERROR_PARITY;
#ifdef DEBUG_STKNX_READ
                        console_print_char('p');
#endif
                    }
                }
                else if(CURRENT_BIT_POSITION(current_bit) == BIT_POSITION_STOP1)
                {
                    /* Full Byte recived */
                    if((_current_bit_state == 0) || (error & ERROR_FRAME))
                    {
                        /* There was a FRAME ERROR during byte reception */
#ifdef DEBUG_STKNX_READ
                        console_print_string("[F]");
#endif
                        Ph_Data__ind(Ind_framing_error, current_rx_byte);
                    }
                    else if(error & ERROR_PARITY)
                    {
                        /* There was a parity error */
#ifdef DEBUG_STKNX_READ
                        console_print_string("[P]");
#endif
                        Ph_Data__ind(Ind_parity_error, current_rx_byte);
                    }
                    else
                    {
                        /* Byte reception is valid, send it to upper layer */
#ifdef DEBUG_STKNX_READ
                        console_print_hex(current_rx_byte);
                        console_print_char(' ');
#endif
                        if(CURRENT_BYTE(current_bit) == STD_FRAME_BYTE_CTRL)
                        {
                            /* This is the begin of a frame */
                            Ph_Data__ind(Ind_start_of_Frame, current_rx_byte);
                            data_len = 1;
                        }
                        else
                        {
                            /* This is the content of a frame */
                            Ph_Data__ind(Ind_inner_Frame_char, current_rx_byte);
                            if(CURRENT_BYTE(current_bit) == STD_FRAME_BYTE_LEN)
                            {
                                /*
                                 * Byte containing LENGHT of the Data (need for Lenght of Frame)
                                 * memorise len to calculate ACK position and response
                                 */
                                data_len = (current_rx_byte & 0x0F);
                            }
                        }

                        if(CURRENT_BYTE(current_bit) < STD_FRAME_SIZE_IN_BYTE(data_len) - 1)
                        {
                            int ccr1 = TIM2->CCR1;
                            int ccr2 = TIM2->CCR2;
                            int ccer = TIM2->CCER;
                            /* Re-synchronize counter on next start bit to avoid clock disturbed */
                            _rsync = 1;
                            TIM2->CR1 &= ~TIM_CR1_CEN; /* Disable timer */
                            TIM2->CNT = (CURRENT_BYTE(current_bit) + 1) * BIT_IN_BYTE * TICK_IN_BIT;
                            TIM2->CCR1 = ccr1;
                            TIM2->CCR2 = ccr2;
                            TIM2->CCER = ccer;
                        }
                    }

                    current_rx_byte = 0;
                    if(error)
                    {
#ifdef DEBUG_STKNX_READ
                        console_print_char('#');
#endif
                        Ph_Knx_Set_Wait_For_Reset();
                    }
                }
                /* Don't return because we need to run the end
                 * in order to prepare for next bit recepiton */
            }


            else if(current_bit - (data_len * BIT_IN_BYTE) == ACK_END + 47)
            {
                /* Reach End of frame */
                if(_tx_priority != Invalid)
                {
                    _begin_of_tx = (data_len * BIT_IN_BYTE) + ((_tx_priority == SystemPriority || _tx_priority == UrgentPriority)?(178):(181));
                    tx_pulse(_begin_of_tx); /* Configure start bit */
                    current_tx_byte = _tx[0];
                    set_state(STATE_TX_IN_PROGRESS);
#ifdef DEBUG_STKNX_WRITE
                    console_print_char('<');
                    console_print_int(_begin_of_tx);
                    console_print_char('>');
#endif
                }
                else
                {
                    set_state(STATE_WAIT_BUS_FREE);
                }
            }

            else if(_need_to_ack)
            {
                static int parity;
                int ackpos = current_bit - (data_len * BIT_IN_BYTE);
                int start_pulse_bit = ((data_len * BIT_IN_BYTE + ackpos + 1) );

                if(ackpos >= (ACK_BEGIN - 1))
                {
                    switch(ackpos)
                    {
                        case ACK_BEGIN - 1: /* Prepare Start Bit */
                        {
#ifdef DEBUG_STKNX_WRITE
                            console_print_char('~');
#endif
                            tx_pulse(start_pulse_bit);
                            parity = 0;
                            break;
                        }
                        case ACK_BEGIN: /* Prepare D0 */
                        case ACK_BEGIN + 1: /* Prepare D1 */
                        case ACK_BEGIN + 2: /* Prepare D2 */
                        case ACK_BEGIN + 3: /* Prepare D3 */
                        case ACK_BEGIN + 4: /* Prepare D4 */
                        case ACK_BEGIN + 5: /* Prepare D5 */
                        case ACK_BEGIN + 6: /* Prepare D6 */
                        case ACK_BEGIN + 7: /* Prepare D7 */
                        {
                            if(_need_to_ack >> (7 - (ackpos - ACK_BEGIN)) & 0x1)
                            {
                                tx_pulse(start_pulse_bit);
                                parity ^= 1;
                            }
                            break;
                        }
                        case ACK_BEGIN + 8: /* Prepare Parity */
                        {
                            if(!parity)
                            {
                                tx_pulse(start_pulse_bit);
                            }
                            _need_to_ack = 0;
                            break;
                        }
                    }
                }
            }
            _current_bit_state = 1;
            break;
        }
        case STATE_TX_IN_PROGRESS:
        {
            static int parity = 0;
            int current_bit = CURRENT_TX_BIT;
            TIM2->CCR1 = (TICK_IN_BIT * ((TIM2->CNT / TICK_IN_BIT) + 1)) + 70; /* Next bit in 104uS */
            switch(CURRENT_BIT_POSITION(current_bit))
            {
                case BIT_POSITION_START:
                    parity = 0;
                case BIT_POSITION_DATA0:
                case BIT_POSITION_DATA1:
                case BIT_POSITION_DATA2:
                case BIT_POSITION_DATA3:
                case BIT_POSITION_DATA4:
                case BIT_POSITION_DATA5:
                case BIT_POSITION_DATA6:
                    /* Need to prepare before the real bit position,
                     * CURRENT_BIT_POSITION(current_bit) == 0 when we are on the Start bit,
                     * at this moment, we need to prepare the bit 0 of data */
                    if(!((current_tx_byte >> (CURRENT_BIT_POSITION(current_bit) )) & 0x1))
                    {
                        tx_pulse(_begin_of_tx + current_bit + 1);
                        parity ^= 1;
                    }
                    break;
                    /* configure d0 to d7 */
                case BIT_POSITION_DATA7:
                    /* configure parity */
                    if(!parity)
                    {
                        tx_pulse(_begin_of_tx + current_bit + 1);
                    }
                    current_tx_byte = _tx[CURRENT_BYTE(current_bit) + 1]; /* Prepare next Byte to send */
                    break;
                case BIT_POSITION_STOP3:
                    /* Configure next start bit if byte not least else wait 50 bit */
                    tx_pulse(_begin_of_tx + current_bit + 1);
                    if(CURRENT_BYTE(current_bit) + 1 >= _tx_len)
                    {
#ifdef DEBUG_STKNX_WRITE
                        console_print_char('$');
#endif
                        Ph_Knx_Set_Wait_For_Reset();
                    }
                    break;
            }
            _current_bit_state = 1;
            break;



        }
        case STATE_NOT_INIT:
        case STATE_BUS_FREE:
            /* NOT NORMAL CASE */
            console_print_string("WARNING: Ph_Knx_Tim_Counter in bad state ");
            console_print_hex(_state);
            console_print_string("\r\n");
            break;
    }
}

void Ph_Knx_Rx_Interrupt(void)
{
    switch(_state)
    {
        case  STATE_WAIT_BUS_FREE:
            /* Bus not free, restart waiting */
            TIM2->CNT = 0; /* Set counter start to 0 */
            break;

        case STATE_BUS_FREE:
            /* begin of New RX frame */
            _need_to_ack = 0;
            set_state(STATE_RX_IN_PROGRESS);
            /* Detect pulse: 0 logic */
            _current_bit_state = 0;
            break;

        case STATE_RX_IN_PROGRESS:
            /* Detect pulse: 0 logic */
            _current_bit_state = 0;
            if(_rsync)
            {
                _rsync = 0;
                ch1_tim_event();
            }
            break;
    }
}

void Ph_Knx_Tim_Counter(int channel)
{
    switch(channel)
    {
        case HAL_TIM_ACTIVE_CHANNEL_1:
        {
            ch1_tim_event();
            break;
        }
        case HAL_TIM_ACTIVE_CHANNEL_2:
        {
            if(_stop_pulse)
            {
                TIM2->CCR2 =_stop_pulse;
                _stop_pulse = 0;
            }
            else
            {
                TIM2->CCER &= ~TIM_CCER_CC2E;
            }
            break;
        }
        default:
            console_print_char('Z');
    }
}


uint8_t Ph_Knx_Get_Wait_For_Reset(void)
{
    return _state == STATE_WAIT_BUS_FREE;
}

uint8_t Ph_Knx_Get_Bus_Free(void)
{
    return _state == STATE_BUS_FREE;
}

void Ph_Knx_Set_Ack(uint8_t p_data)
{
    _need_to_ack = p_data;
}

void Ph_Knx_Start_Transmit(Priority priority, volatile uint8_t *data, uint8_t len)
{
#ifdef DEBUG_STKNX_WRITE
    console_print_char('!');
#endif
    _tx_priority = priority;
    _tx = data;
    _tx_len = len;
}
