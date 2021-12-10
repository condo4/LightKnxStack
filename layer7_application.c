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
void A_GroupValue_Read__ind(uint8_t ASAP, Priority priority, uint8_t hop_count_type)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("A_GroupValue_Read__ind(");
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
void A_GroupValue_Write__ind(uint8_t ASAP, Priority priority, uint8_t hop_count_type, uint8_t *data,
                             uint8_t octet_count)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("A_GroupValue_Write__ind(");
    console_print_int(ASAP);
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
void A_GroupValue_Read__Acon(uint8_t ASAP, Priority priority, uint8_t hop_count_type, uint8_t *data,
                             uint8_t octet_count)
{
#if defined(DEBUG_LAYER_APPLICATION)
    console_print_string("A_GroupValue_Read__Acon(");
    console_print_int(ASAP);
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
    console_print_string("A_IndividualAddress_Read__ind(");
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
    console_print_string("A_IndividualAddress_Write__ind(");
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
    console_print_string("A_IndividualAddress_Read__res(");
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



void A_Connect__ind(KnxAddress source)
{

}

