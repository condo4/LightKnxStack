/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __LAYER1_PHYSICAL__
#define __LAYER1_PHYSICAL__

/* Physical Layer type TP1 */
#include <stdint.h>

#define FRAME_ACK 0xCC
#define FRAME_NACK 0x0C
#define FRAME_BUSY 0xC0
#define FRAME_NAK_BUSY 0x00

typedef enum
{
    Req_start_of_Frame,
    Req_inner_Frame_char,
    Req_ack_char,
    Req_poll_data_char,
    Req_fill_char,
    Req_end_of_Frame /* Add by me, need for NCN5130 */
} Ph_Data_Req_Class;

typedef enum
{
    Con_OK,
    Con_bus_not_free,
    Con_collision_detected,
    Con_transceiver_fault
} Ph_Data_Con_Class;

typedef enum
{
    Ind_start_of_Frame,
    Ind_inner_Frame_char,
    Ind_ack_char,
    Ind_poll_data_char,
    Ind_parity_error,
    Ind_framing_error,
    Ind_bit_error
} Ph_Data_Ind_Class;

typedef enum
{
    p_ok = 0x0,
    p_transceiver_fault = 0x1,
    p_collision_detected = 0x2,
    P_bus_not_free = 0x3,
} P_Status;


typedef enum
{
    l_ok = 0x0,
    l_not_ok = 0x1
} L_Status;


void Ph_Init(void);
void Ph_Loop(void);

/* 1.4.1 Ph_Data Service */

/**
 * \fn Ph_Data.req(p_class, p_data)
 * \desc The Ph_Data.req primitive is applied by the Physical Layer user to pass user data
 * consisting of an octet via the p_data parameter to the Physical Layer type TP1. The p_class
 * parameter describes the transmission task to be executed by the Physical Layer type TP1 entity.
 * \param p_class
 *    start_of_Frame: transmit first character of a Frame with bus free detection at the start bit.
 *    inner_Frame_char: transmit character after two bit times without bus free detection at the
 * start bit ack_char: transmit character 15 bit times after the preceding character received
 * without bus free detection at the start bit poll_data_char: transmit character 5 bit times after
 * the preceding character received without bus free detection at the start bit fill_char: transmit
 * character 6 bit times after previous character received with bus free detection at the start bit
 * \param p_data octet: the octet to be converted to a UART character and to be transmitted.
 */
void Ph_Data__req(Ph_Data_Req_Class p_class, uint8_t p_data);

/**
 * \fn Ph_Data.con(p_status)
 * The Ph_Data.con primitive passes status information via the parameter p_status back to the
 * Physical Layer user. The value of p_status indicates whether the transmission of the contents of
 * the p_data parameter previously passed to the Physical Layer type TP1 entity via the Ph_Data.req
 * primitive succeeded. \param p_status OK: character transmission succeeded bus_not_free: no
 * transmission, another device is transmitting collision_detected: a collision was detected
 * (logical ‘1’ transmitted, but logical  ́0 ́ received) transceiver_fault: transceiver fault detected
 */
void Ph_Data__con(P_Status p_status);

/**
 * \fn Ph_Data.ind(p_class, p_data)
 * The Ph_Data.ind primitive passes timing information via the parameter p_class and user data via
 * the parameter p_data from the Physical Layer type TP1 entity to the Physical Layer user. See
 * clause 1.5 for more details. \parm p_class: start_of_Frame: first character of request Frame
 * received. inner_Frame_char: character received at 2 bit times after the preceding one ack_char:
 * character received 15 bit times after the preceding one poll_data_char: character received at
 * five or six bit times after the preceding one parity_error: wrong parity bit detected in the
 * character received framing_error: wrong stop bit detected in the character received bit_error:
 * wrong data bit detected in the character. Data bit did not meet the bit decoding rules. \param
 * p_data octet: the data octet extracted from the received character
 */
void Ph_Data__ind(Ph_Data_Ind_Class p_class, uint8_t p_data);

/* 1.4.2 Ph_Reset Service
 * The Ph_Reset service shall be applied by the user of Physical Layer during start up, in order to
 * synchronize to possibly existing network traffic.
 */

/**
 * \fn Ph_Reset.req()
 * \desc Start synchronisation activity
 */
void Ph_Reset__req();

/**
 * \fn Ph_Reset.con(p_status);
 * \param p_status OK: a bus free time of 50 bit times was detected
 * transceiver_fault: undefined physical signal detected
 */
void Ph_Reset__con(P_Status p_status);

#endif
