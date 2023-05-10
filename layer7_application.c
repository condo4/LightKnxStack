/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "layer7_application.h"
#include "layer4_transport.h"
#include "object_device.h"
#include "debug.h"

/**
 * \fn A_GroupValue_Read.ind(ASAP, priority, hop_count_type)
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 */
void A_GroupValue_Read__ind(uint16_t ASAP, Priority priority, uint8_t hop_count_type)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_GroupValue_Read__ind(");
    console_print_int(ASAP);
    console_print_char(',');
    print_priority(priority);
    console_print_char(',');
    console_print_int(hop_count_type);
    console_print_string(")\n");
#endif
}

/**
 * \fn A_GroupValue_Write.ind(ASAP, priority, hop_count_type, data)
 * \param ASAP: local reference of the Service Access Point
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param data: data of the associated application layer Service Access Point
 */
void A_GroupValue_Write__ind(uint16_t ASAP, Priority priority, uint8_t hop_count_type, uint8_t *data,
                             uint8_t octet_count)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_GroupValue_Write__ind(");
    console_print_dev_addr(ASAP);
    console_print_char(',');
    print_priority(priority);
    console_print_char(',');
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_char('[');
    for (int i = 0; i < octet_count; i++)
        console_print_hex(data[i]);
    console_print_char(']');
    console_print_char('\r');
    console_print_char('\n');
#endif
}

/**
 * \fn A_GroupValue_Read.Acon(ASAP, priority, hop_count_type, data)
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 * \param data: the value of the associated Service Access Point
 * \param octet_count: Need for check and debug only
 */
void A_GroupValue_Read__Acon(uint16_t ASAP, Priority priority, uint8_t hop_count_type, uint8_t *data,
                             uint8_t octet_count)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_GroupValue_Read__Acon(");
    console_print_dev_addr(ASAP);
    console_print_char(',');
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", [0x");
    for (int i = 0; i < octet_count; i++)
    {
        console_print_hex(data[i]);
        console_print_char(',');
    }
    console_print_hex(data[octet_count]);
    console_print_string("])\n");
#endif
}

/**
 * \fn A_IndividualAddress_Read.ind(priority, hop_count_type)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 */
void A_IndividualAddress_Read__ind(Priority priority, uint8_t hop_count_type)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_IndividualAddress_Read__ind(");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(")\r\n");
#endif
    A_IndividualAddress_Read__res(0, priority, hop_count_type, device_induvidual_address());
}

void A_IndividualAddress_Write__ind(Priority priority, uint8_t hop_count_type,
                                    KnxAddress newaddress)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_IndividualAddress_Write__ind(");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    print_source_address(newaddress);
    console_print_string(") TODO ACK RESP\r\n");
#endif
    device_induvidual_address_set(newaddress);
}

/**
 * \fn A_IndividualAddress_Read.res(ack_request, priority, hop_count_type, Individual Address)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param individual_address: the value of the Individual Address
 */
void A_IndividualAddress_Read__res(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                   KnxAddress individual_address)
{
    static uint8_t pdu[2] = { A_IndividualAddress_Response >> 8,
                              A_IndividualAddress_Response & 0xFF };
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_IndividualAddress_Read__res(");
    console_print_int(ack_request);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    print_source_address(individual_address);
    console_print_string(")\r\n");
#endif

    T_Data_Broadcast__req(ack_request, hop_count_type, 2, priority, pdu);
}

/**
 * \fn A_IndividualAddress_Read.ind(priority, hop_count_type)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 */
void A_DomainAddress_Read__ind(Priority priority, uint8_t hop_count_type)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_DomainAddress_Read__ind(");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(") Do Nothing in TP\r\n");
#endif
    /* Not need to respond in TP */
}

/**
 * \fn A_IndividualAddress_Read.res(ack_request, priority, hop_count_type, Individual Address)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param individual_address: the value of the Individual Address
 */
void A_DomainAddress_Read__res(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                   uint16_t domain_address)
{
    static uint8_t pdu[4] = { A_DomainAddress_Response >> 8,
                             A_DomainAddress_Response & 0xFF, 0, 0 };
    pdu[2] = (domain_address >> 8) & 0xFF;
    pdu[3] = domain_address & 0xFF;
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_DomainAddress_Read__res(");
    console_print_int(ack_request);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    print_source_address(domain_address);
    console_print_string(")\r\n");
#endif

    T_Data_Broadcast__req(ack_request, hop_count_type, 4, priority, pdu);
}


void A_Connect__ind(KnxAddress source)
{
    // Do Nothing
}

/**
 * \fn A_DeviceDescriptor_Read.ind(priority, hop_count_type, ASAP, descriptor_type)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param ASAP: local reference of the Service Access Point
 * \param descriptor_type type of the Device Descriptor
 */
void A_DeviceDescriptor_Read__ind(Priority priority, uint8_t hop_count_type, uint16_t ASAP, uint8_t descriptor_type)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_DeviceDescriptor_Read__ind(");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    print_source_address(ASAP);
    console_print_string(", ");
    console_print_hex(descriptor_type);
    console_print_string(")\r\n");
#endif
    switch(descriptor_type)
    {
    case 0:
        A_DeviceDescriptor_Read__res(1, priority, hop_count_type, ASAP, descriptor_type, DeviceDescriptor_SystemB);
        break;
    default:
        console_print_string("TODO: A_DeviceDescriptor_Read__ind with type = ");
        console_print_hex(descriptor_type);
        console_print_string(" not supported\r\n");
        break;
    }
}

void A_DeviceDescriptor_Read__res(uint8_t ack_request, Priority priority, uint8_t hop_count_type, uint16_t ASAP, uint8_t descriptor_type,
                                  uint16_t device_descriptor)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("     A_DeviceDescriptor_Read__res(");
    console_print_int(ack_request);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    print_source_address(ASAP);
    console_print_string(", ");
    console_print_hex(descriptor_type);
    console_print_string(", ");
    console_print_hex(device_descriptor >> 8);
    console_print_hex(device_descriptor & 0xFF);
    console_print_string(")\r\n");
#endif
    uint8_t tsdu[4];
    tsdu[0] = (A_DeviceDescriptor_Response >> 8);
    tsdu[1] = (A_DeviceDescriptor_Response & 0xC0);
    tsdu[1] |= descriptor_type;
    tsdu[2] = device_descriptor >> 8;
    tsdu[3] = device_descriptor & 0xFF;
    if(T_Data_IsConnected())
    {
        T_Data_Connected__req(4, priority, ASAP, tsdu);
    }
    else
    {
        T_Data_Individual__req(ack_request, hop_count_type, 4, priority, ASAP, tsdu);
    }
}
