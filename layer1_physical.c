/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdbool.h>
#include <stddef.h>
#include <main.h>
#include <usart.h>
#include <tim.h>
#include <console.h>
#include <layer1_physical.h>
#include <layer2_data_link.h>
#include <platform.h>
//#include <object_device.h>
#include <debug.h>

#ifdef DEBUG_LAYER_PHYSICAL
#define DEBUG
#endif

#define STATE_RESET         0
// Entered after Power On Reset (POR) or in response to a U_Reset__req() service issued by the host controller. In this
// state NCN5130 gets initialized, all features disabled and services are ignored and not executed.

// Entered after Reset State or when VBUS, VFILT or Xtal are not operating correctly (operation of VBUS, VFILT and
// XTAL can be verified by means of the System Status Service, p37). Communication with KNX bus is not allowed.
// U_SystemStat.ind can be used to verify this state (code 00).
#define STATE_POWER_UP      1
#define STATE_POWER_UP_STOP 2

// This state is useful for setting−up NCN5130 safely or temporarily interrupting reception from the KNX bus.
// U_SystemStat.ind can be used to verify this state (code 10).
#define STATE_STOP          3

// NCN5130 remains in this state until it detects silence on the KNX bus for at least 40 Tbits.
// Although the receiver of NCN5130 is on, no frames are transmitted to the host controller.
// U_SystemStat.ind can be used to verify this state (code 01).
#define STATE_SYNC          4

// In this state the device is fully functional. Communication with the KNX bus is allowed.
// U_SystemStat.ind can be used to verify this state (code 11).
#define STATE_NORMAL        5

static unsigned char m_state = STATE_RESET;
static unsigned char m_send_msg[4];
static unsigned char m_recive_msg[64];
static unsigned char m_rx_index = 0;
static unsigned char m_tx_index = 0;
static unsigned char m_to_send = 0;


#define RX_BUS_BUSY -1
#define RX_BUS_FREE 0
#define RX_FRAME_IN_PROPGRESS 1;
static int m_rx_state = RX_BUS_BUSY;



/* SERVICES FROM HOST CONTROLLER */
void send_to_controler(size_t size)
{
    m_to_send = size;
    m_tx_index = 0;

#ifdef DEBUG_LAYER_PHYSICAL_TX
    console_print_char('%');
    console_print_bytes(m_send_msg, size);
    console_print_char(' ');
#endif
    device_uart_tx(&m_send_msg[m_tx_index]);
}


void device_uart_tx_confirm()
{
    /* !!! ISR context !!! */
    m_tx_index++;
    if(m_tx_index < m_to_send)
    {
        device_uart_tx(&m_send_msg[m_tx_index]);
        return;
    }

    /* EOT */
    m_tx_index = 0;
    m_to_send = 0;

    Ph_Data__con(l_ok);
}

void U_Reset__req()
{
#ifdef DEBUG
    console_print_string("U_Reset__req\r\n");
#endif
    m_state = STATE_RESET;
    m_send_msg[0] = 0x01;
    send_to_controler(1);
}

void U_State__req()
{
    m_send_msg[0] = 0x02;
    send_to_controler(1);
}

void U_SetBusy__req()
{
    m_send_msg[0] = 0x03;
    send_to_controler(1);
}

void U_QuitBusy__req()
{
    m_send_msg[0] = 0x04;
    send_to_controler(1);
}

void U_Busmon__req()
{
    m_send_msg[0] = 0x05;
    send_to_controler(1);
}

void U_SetAddress__req(unsigned char addr_low, unsigned char addr_hight)
{
    m_send_msg[0] = 0xF1;
    m_send_msg[1] = addr_hight;
    m_send_msg[2] = addr_low;
    send_to_controler(4);
}


void U_SetRepetition__req(unsigned char rep_cntrs)
{
    m_send_msg[0] = 0xF2;
    m_send_msg[1] = rep_cntrs;
    send_to_controler(4);
}

void U_L_DataOffset__req(unsigned char index)
{
    m_send_msg[0] = 0x08 | (index & 0x07);
    send_to_controler(1);
}

void U_SystemStat__req()
{
    m_send_msg[0] = 0x0D;
    send_to_controler(1);
}

void U_StopMode__req()
{
    m_send_msg[0] = 0x0E;
    send_to_controler(1);
}

void U_ExitStopMode__req()
{
    m_send_msg[0] = 0x0F;
    send_to_controler(1);
}

void U_Ackn__req(bool nack, bool busy, bool addressed)
{
    m_send_msg[0] = 0x10 | (nack << 2) | (busy << 1) | addressed;
#ifdef DEBUG
    console_print_string("{ACK:");
    console_print_hex(m_send_msg[0]);
    console_print_char('}');
#endif
    send_to_controler(1);
}

void U_Configure__req(bool auto_polling, bool crc_ccitt, bool frame_end_marker)
{
    m_send_msg[0] = 0x18 | (auto_polling << 2) | (crc_ccitt << 1) | frame_end_marker;
    send_to_controler(1);
}

void U_IntRegWr__req(unsigned char address, unsigned char data)
{
    m_send_msg[0] = 0x28 | (address & 0x3);
    m_send_msg[1] = data;
    send_to_controler(2);
}

void U_IntRegRd__req(unsigned char address)
{
    m_send_msg[0] = 0x38 | (address & 0x3);
    send_to_controler(1);
}

void U_PollingState__req(unsigned char slot, unsigned char addr_hight, unsigned char addr_low, unsigned char state)
{
    m_send_msg[0] = 0xE0 | (slot & 0x0F);
    m_send_msg[1] = addr_hight;
    m_send_msg[2] = addr_low;
    m_send_msg[3] = state;
    send_to_controler(4);
}


void U_L_DataStart__req(unsigned char ctrl)
{
    m_send_msg[0] = 0x80;
    m_send_msg[1] = ctrl;
    send_to_controler(2);
}

void U_L_DataCont__req(unsigned char index, unsigned char data)
{
    m_send_msg[0] = 0x80 | (index & 0x3F);
    m_send_msg[1] = data;
    send_to_controler(2);
}

void U_L_DataEnd__req(unsigned char index, unsigned char fcs)
{
    m_send_msg[0] = 0x40 | (index & 0x3F);
    m_send_msg[1] = fcs;
    send_to_controler(2);
}


/*      CONTROL SERVICES – DEVICE SPECIFIC */

void U_Reset__ind() // Called from hardware
{
#ifdef DEBUG
    console_print_string("U_Reset__ind\r\n");
#endif
    m_state = STATE_NORMAL;
    m_rx_state = RX_BUS_FREE;
    Ph_Reset__con(p_ok);
}



void U_State__ind(bool slave_collision, bool receive_error, bool transmit_error, bool protocol_error, bool temperature_warning)
{
    // TODO:
    console_print_string("U_State__ind: ");
    if(slave_collision) console_print_string("slave_collision ");
    if(receive_error) console_print_string("receive_error ");
    if(transmit_error) console_print_string("transmit_error ");
    if(protocol_error) console_print_string("protocol_error ");
    if(temperature_warning) console_print_string("temperature_warning ");
    console_print_string("\r\n");
}

void U_FrameState__ind(bool bit_error, bool crc_or_lenght_error, bool timing_error)
{
    // TODO:
    if(1 | bit_error | crc_or_lenght_error | timing_error)
    {
        console_print_string("U_FrameState__ind: ");
        if(bit_error) console_print_string("bit_error ");
        if(crc_or_lenght_error) console_print_string("crc_or_lenght_error ");
        if(timing_error) console_print_string("timing_error ");
        console_print_string("\r\n");
    }
}

void U_Configure__ind(bool auto_ack, bool auto_poll, bool crc_ccitt, bool frame_end_marker)
{
    // TODO:
}

void U_FrameEnd__ind()
{
    // TODO:
}

void U_StopMode__ind()
{
    m_state = STATE_STOP;
    // TODO
}

void U_SystemStat__ind(unsigned char byte)
{
    // TODO:
}


void EOF_TIMER_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if(htim == &htim6)
    {
        if(htim6.Instance->CR1 && TIM_CR1_CEN)
        {
#ifdef DEBUG
            console_print_string("#\r\n");
#endif
            HAL_TIM_Base_Stop(&htim6);
            __HAL_TIM_SET_COUNTER(&htim6, 1);

            if((m_recive_msg[0] & 0xD3) == 0x90)
            {
                /* Standard Frame recived */
#ifdef DEBUG_LAYER_DATA_LINK
    console_print_string("> ");
    console_print_bytes((uint8_t *)m_recive_msg, m_rx_index);
    console_print_string("\r\n");
#endif

    
                L_Data__ind(
                    1,
                    ((m_recive_msg[5] & 0x80) >> 7),
                    ((m_recive_msg[3] << 8) | m_recive_msg[4]),
                    ((m_recive_msg[0] & 0x80) >> 7),
                    &m_recive_msg[5],
                    (m_recive_msg[5] & 0x0F) + 2,
                    (m_recive_msg[0] & 0x0C) >> 2,
                    ((m_recive_msg[1] << 8) | m_recive_msg[2])
                );
            }

            m_rx_index = 0;
        }
    }
}






void Ph_Init(void)
{
    MX_TIM6_Init();

    htim6.PeriodElapsedCallback = EOF_TIMER_PeriodElapsedCallback;

    __HAL_TIM_SET_COUNTER(&htim6, 1);
    HAL_TIM_Base_Start_IT(&htim6); /* Don't understand why we need this */

    while(HAL_GPIO_ReadPin(KNX_SAVEB_GPIO_Port, KNX_SAVEB_Pin) == 0)
    {
        // Wait for SAVEB
    }

    device_uart_init();
}

void Ph_Loop(void)
{
    static bool knx_btn_state = false;

    if (HAL_GPIO_ReadPin(KNX_PROG_BTN_GPIO_Port, KNX_PROG_BTN_Pin) == GPIO_PIN_RESET)
    {
        if (!knx_btn_state)
        {
            knx_btn_state = true;
            device_prog_mode_set(!device_prog_mode());
        }
    }
    else
    {
        if (knx_btn_state)
        {
            knx_btn_state = false;
        }
    }
}

void Ph_Reset__req()
{
    U_Reset__req();
}


void Ph_Data__con(P_Status p_status)
{
    /* !!! ISR context !!! */
    L_Data__con(0,0,0,0,0,0,0,l_ok);
}

void Ph_Data__req(Ph_Data_Req_Class p_class, uint8_t p_data)
{
    /* Since NCN5130 Buffering of Sent Data Frames, consider always free*/
    /* Start to transmit first byte */
    static int l_index = 0;
    l_index++;
    switch(p_class)
    {
        case Req_start_of_Frame:
            U_L_DataStart__req(p_data);
            l_index = 0;
            break;
        case Req_inner_Frame_char:
            U_L_DataCont__req(l_index, p_data);
            break;
        case Req_end_of_Frame:
            U_L_DataEnd__req(l_index, p_data);
            break;
        case Req_ack_char:
            U_Ackn__req(/* nack */ true, /* busy */ false, /* addressed */ true);
            break;
        case Req_poll_data_char:
            /* TODO */
            break;
        case Req_fill_char:
            /* TODO */
            break;
    }
}

void device_uart_rx(unsigned char byte)
{
    /* !!! ISR context !!! */
    if(htim6.Instance->CR1 && TIM_CR1_CEN)
    {
        HAL_TIM_Base_Stop(&htim6);
    }
    m_recive_msg[m_rx_index++] = byte;

#ifdef DEBUG
    console_print_hex(byte);
    console_print_char(' ');
#endif

    /* DLL (LAYER 2) SERVICES (DEVICE IS TRANSPARENT) */
    if((m_recive_msg[0] & 0xD3) == 0x90)
    {
        __HAL_TIM_SET_COUNTER(&htim6, 1);
        HAL_TIM_Base_Start_IT(&htim6);
        return;
    }
    else if((m_recive_msg[0] & 0xD3) == 0x10)
    {
        __HAL_TIM_SET_COUNTER(&htim6, 1);
        HAL_TIM_Base_Start_IT(&htim6);
        return;
    }
    else if(m_recive_msg[0] == 0xF0)
    {
        /* TODO */
        console_print_string("TODO: device_uart_rx 0xF0\r\n");
        //L_Poll_Data__ind(&m_recive_msg[1], m_rx_index - 1);
    }
    /*  ACKNOWLEDGE SERVICES (DEVICE IS TRANSPARENT IN BUS MONITOR MODE) */
    else if((m_recive_msg[0] & 0x33) == 0x00)
    {
        //L_Ackn__ind(m_recive_msg[0]);
    }
    else if((m_recive_msg[0] & 0x7F) == 0x0B)
    {
        if(m_recive_msg[0] & 0x80)
            L_Data__con(0,0,0,0,0,0,0,l_ok);
        else
            L_Data__con(0,0,0,0,0,0,0,l_not_ok);
    }
    /*  CONTROL SERVICES – DEVICE SPECIFIC */
    else if(m_recive_msg[0] == 0x03)
    {
        U_Reset__ind();
    }
    else if((m_recive_msg[0] & 0x07) == 0x07)
    {
        U_State__ind(
            (m_recive_msg[0] & 0x80) >> 7,
            (m_recive_msg[0] & 0x40) >> 6,
            (m_recive_msg[0] & 0x20) >> 5,
            (m_recive_msg[0] & 0x10) >> 4,
            (m_recive_msg[0] & 0x08) >> 3
        );
    }
    else if((m_recive_msg[0] & 0x17) == 0x13)
    {
        U_FrameState__ind(
            (m_recive_msg[0] & 0x80) >> 7,
            (m_recive_msg[0] & 0x40) >> 6,
            (m_recive_msg[0] & 0x20) >> 5
        );
    }
    else if((m_recive_msg[0] & 0x83) == 0x01)
    {
        U_Configure__ind(
            (m_recive_msg[0] & 0x20) >> 5,
            (m_recive_msg[0] & 0x10) >> 4,
            (m_recive_msg[0] & 0x08) >> 3,
            (m_recive_msg[0] & 0x04) >> 2
        );
    }
    else if(m_recive_msg[0] == 0xCB)
    {
        U_FrameEnd__ind();
    }
    else if(m_recive_msg[0] == 0x2B)
    {
        U_StopMode__ind();
    }
    if(m_recive_msg[0] == 0x4B)
    {
        U_SystemStat__ind(m_recive_msg[1]);
    }

    /* Ready for next frame */
    m_rx_index = 0;
}
