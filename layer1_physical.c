#include "layer1_physical.h"
#include "layer2_data_link.h"
#include "knx_types.h"
#include "console.h"
#include <tim.h>

#define DEBUG_PHYSICAL_LAYER

uint8_t RxFrame[MAX_KNX_TELEGRAM_SIZE] = {
    0,
};
volatile uint8_t xor_rx = 0;
volatile uint8_t *pt;
volatile uint8_t ack_ready = 0;

volatile uint8_t reset_asked = 0;

volatile uint8_t error = 0;
#define ERROR_PARITY 0
#define ERROR_FRAMING 1
#define ERROR_BIT 2
#define ERROR_OVERFLOW 3

#define IS_L_DATA_FRAME(lsdu) ((lsdu[0] & 0x53) == 0x10)
#define IS_L_POLL_DATA_FRAME(lsdu) ((lsdu[0] & 0x53) == 0x50)
#define IS_L_ACK_FRAME(lsdu) ((lsdu[0] & 0x53) == 0x00)
#define L_FRAME_TYPE(lsdu) ((lsdu[0] >> 7) & 0x1)
#define L_REPEAT_FLAG(lsdu) ((lsdu[0] >> 5) & 0x1)
#define L_PRIORITY(lsdu) ((lsdu[0] >> 2) & 0x3)
#define L_DATA_SA(lsdu) ((lsdu[1] << 8) | lsdu[2])
#define L_DATA_DA(lsdu) ((lsdu[3] << 8) | lsdu[4])
#define L_DATA_AT(lsdu) ((lsdu[5] >> 7) & 0x1)
#define L_DATA_HOP(lsdu) ((lsdu[5] >> 4) & 0x7)
#define L_DATA_LENGTH(lsdu) ((lsdu[5] & 0xF))

#define L_DATA_STD_COMPLET                                                                         \
    ((RxFrame[5] != 0) && (L_FRAME_TYPE(RxFrame) == L_Data_Standard) && (RxFrame != pt)            \
      && (pt == (RxFrame + 6 + L_DATA_LENGTH(RxFrame) + 2)))

//#define L_DATA_STD_COMPLET  ((RxFrame[5] != 0) && (L_FRAME_TYPE(RxFrame) == L_Data_Standard) && (RxFrame != pt) && (L_DATA_LENGTH(RxFrame) != 0) && (pt == (RxFrame + 6 + L_DATA_LENGTH(RxFrame) + 2) ))

void Ph_Data__req(Ph_Data_Req_Class p_class, uint8_t p_data)
{
    if (p_class == Req_start_of_Frame)
    {
        /* Bus free detection */
        if (!Ph_Knx_Get_Bus_Free())
        {
            Ph_Data__con(P_bus_not_free);
            return;
        }
        /* Start to transmit first byte */
    }

    if (p_class == Req_ack_char)
    {
        Ph_Knx_Set_Ack(p_data);
    }

    /* TODO: Send a Byte to the Bus, when it's OK, call Ph_Data__Confirm */
    /* Ph_Data__Confirm will send the next one */
}

void Ph_Data__con(P_Status p_status)
{
    /*
L_Data__con(
    AddressType address_type,
    KnxAddress destination_address,
    FrameFormat frame_format,
    uint8_t octet_count,
    Priority priority,
    KnxAddress source_address,
    uint8_t *lsdu,
    L_Status l_status
)
    */
}

void Ph_Data__ind(Ph_Data_Ind_Class p_class, uint8_t p_data)
{
    switch (p_class)
    {
    case Ind_start_of_Frame:
        xor_rx = 0;
        pt = RxFrame;
        error = 0;
    case Ind_inner_Frame_char:
        if (pt < RxFrame + MAX_KNX_TELEGRAM_SIZE)
        {
            *pt++ = p_data;
            xor_rx ^= p_data;
            if (L_DATA_STD_COMPLET)
            {
                if (xor_rx == 0xFF)
                {
                    /* RX OK */
//#if defined(DEBUG_LAYER_PHYSICAL)
    console_print_string("LPDU: ");
    for (int i = 0; i < L_DATA_LENGTH(RxFrame) + 8; i++)
    {
        console_print_hex(RxFrame[i]);
        console_print_char(' ');
    }
    console_print_string(" :");

    if((RxFrame[5] != 0)) console_print_char('1');
    else console_print_char('0');
    if((L_FRAME_TYPE(RxFrame) == L_Data_Standard)) console_print_char('1');
    else console_print_char('0');
    if((RxFrame != pt)) console_print_char('1');
    else console_print_char('0');
    console_print_char('<');
    console_print_int(L_DATA_LENGTH(RxFrame));
    console_print_char('>');
    if((pt == (RxFrame + 6 + L_DATA_LENGTH(RxFrame) + 2))) console_print_char('1');
    else console_print_char('0');

//BC 11 2B  2A 21  E5 00 80 00 00 0A 60 7D



    //#endif

                    if (IS_L_DATA_FRAME(RxFrame))
                    {
                        L_Data__ind(1, L_DATA_AT(RxFrame), L_DATA_DA(RxFrame),
                                    L_FRAME_TYPE(RxFrame), &RxFrame[5], L_DATA_LENGTH(RxFrame) + 2,
                                    L_PRIORITY(RxFrame), L_DATA_SA(RxFrame));
                    }
                    else if (IS_L_ACK_FRAME(RxFrame))
                    {
                    }
                    else if (IS_L_POLL_DATA_FRAME(RxFrame))
                    {
                    }

                    pt = RxFrame;
                    xor_rx = 0;
                    RxFrame[5] = 0;
                }
                else
                {
                }
            }
            else
            {
            }
        }
        else
        {
            error |= (1 << ERROR_OVERFLOW);
        }
        break;
    case Ind_ack_char:
        ack_ready = 1;
        break;
    case Ind_poll_data_char:
        // printf("POLL> %02x\n", p_data);
        break;
    case Ind_parity_error:
        error |= (1 << ERROR_PARITY);
        return;
    case Ind_framing_error:
        error |= (1 << ERROR_FRAMING);
        return;
    case Ind_bit_error:
        error |= (1 << ERROR_BIT);
        return;
    }
    if (error)
    {
        console_print_string("ERROR ");
        console_print_int(error);
        console_print_string("%x\n");
    }
}

void Ph_Init(void)
{
    MX_TIM2_Init();

    /* Enable interrupt for TIM2 Compare */
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);
    htim2.Instance->CCER = 0x1;
}

void Ph_Reset__req()
{
    Ph_Knx_Set_Wait_For_Reset();
    while (Ph_Knx_Get_Wait_For_Reset() == 1)
    {
        // printf("Wait Reset\n");
        HAL_Delay(1);
    }
    Ph_Reset__con(p_ok);
}
