#include "layer4_transport.h"
#include "layer7_application.h"
#include "layer3_network.h"
#include "object_association_table.h"
#include "object_device.h"
#include "debug.h"

/**
 * \fn T_Data_Group.ind(hop_count_type, octet_count, priority, source_address, TSAP, tsdu)
 * \param hop_count_type: hop count equals 7 or not
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: system, urgent, normal or low priority
 * \param source_address: the Individual Address of the originator of the message
 * \param TSAP: identifier of the service access point
 * \param tsdu: this is the user data that has been transferred by Transport Layer
 */
void T_Data_Group__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                       KnxAddress source_address, uint8_t TSAP, uint8_t *tsdu)
{
    uint16_t index = 0;
    int32_t asap = 0;
    uint16_t APCI = (tsdu[0] << 8 | tsdu[1]);

#if defined(DEBUG_LAYER_TRANSPORT)
    console_print_string("T_Data_Group__ind(");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(", ");
    console_print_int(TSAP);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("]); APCI=");
    console_print_int(APCI);
    console_print_string("\r\n");
#endif

    do
    {
        asap = association_table_next_asap(TSAP, &index);
        if (asap == -1)
            return;

        switch (APCI)
        {
        case A_GroupValue_Read:
            A_GroupValue_Read__ind(asap, priority, hop_count_type);
            break;
        case A_GroupValue_Response:
            A_GroupValue_Read__Acon(asap, priority, hop_count_type, &tsdu[1], octet_count - 1);
            break;
        case A_GroupValue_Write:
            A_GroupValue_Write__ind(asap, priority, hop_count_type, &tsdu[1], octet_count - 1);
            break;
        default:
            console_print_string("WARNING: Unknonw 0x");
            console_print_hex(APCI);
            console_print_char('\r');
            console_print_char('\n');
            break;
        }
    } while (asap != -1);
}

/**
 * \fn T_Data_Individual.ind(hop_count_type, octet_count, priority, TSAP, tsdu)
 * \param hop_count_type: hop count equals 7 or not
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point (may be direct the Individual Address of the
 * remote partner) \param tsdu: is the user data that has been transferred by Transport Layer
 */
void T_Data_Individual__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                            uint8_t TSAP, uint8_t *tsdu)
{
#ifdef DEBUG_LAYER_TRANSPORT
    console_print_string("T_Data_Individual__ind(");
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_int(octet_count);
    console_print_char(',');
    print_priority(priority);
    console_print_char(',');
    print_source_address(TSAP);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("]\r\n");
#endif

    uint16_t APCI = (tsdu[0] << 8 | tsdu[1]);
    /* Becarrful, GroupValuen ADC, Memory lower bit is data */

    switch (APCI)
    {
    case A_DeviceDescriptor_Read:
        // TODO
        break;
    case A_Restart:
        // TODO
        break;
    case A_FileStream_InfoReport:
        // TODO
        break;
    case A_PropertyValue_Read:
        // TODO
        break;
    case A_PropertyValue_Response:
        // TODO
        break;
    case A_PropertyValue_Write:
        // TODO
        break;
    case A_PropertyDescription_Read:
        // TODO
        break;
    case A_PropertyDescription_Response:
        // TODO
        break;
    case A_Link_Read:
        // TODO
        break;
    case A_Link_Response:
        // TODO
        break;
    case A_Link_Write:
        // TODO
        break;
    case A_FunctionPropertyCommand:
        // TODO
        break;
    case A_FunctionPropertyState_Response:
        // TODO
        break;
    case A_FunctionPropertyState_Read:
        // TODO
        break;
        break;
    case A_NetworkParameter_Read:
        // TODO
        break;
    case A_NetworkParameter_Response:
        // TODO
        break;
    case A_NetworkParameter_Write:
        // TODO
        break;
    /*
    case  A_NetworkParameter_InfoReport:
        break;
    */
    default:
        console_print_string("WARNING: Unknonw 0x");
        console_print_hex(APCI);
        console_print_string("\r\n");
        break;
    }
}

/**
 * \fn T_Data_Broadcast.req(ack_request, hop_count_type, octet_count, priority, tsdu)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type: hop count 7 or Network Layer Parameter
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param tsdu: This shall be the user data to be transferred by Transport Layer
 */
void T_Data_Broadcast__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *tsdu)
{
#ifdef DEBUG_LAYER_TRANSPORT
    console_print_string("T_Data_Broadcast__req(");
    console_print_int(ack_request);
    console_print_char(',');
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_int(octet_count);
    console_print_char(',');
    print_priority(priority);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif
    N_Data_Broadcast__req(ack_request, hop_count_type, octet_count, priority, tsdu);
}

/**
 * \fn T_Data_Broadcast.ind(hop_count_type, octet_count, priority, source_address, tsdu)
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority highest, urgent, normal or low priority
 * \param source_address Individual Address of the device that requested the T_Data_Broadcast
 * service \param tsdu This shall be the user data that has been transferred by Transport Layer.
 */
void T_Data_Broadcast__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, uint8_t *tsdu)
{
#ifdef DEBUG_LAYER_TRANSPORT
    console_print_string("T_Data_Broadcast__ind(");
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_int(octet_count);
    console_print_char(',');
    print_priority(priority);
    console_print_char(',');
    print_source_address(source_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif

    priority = SystemPriority;
    uint16_t pdu = tsdu[0] << 8 | tsdu[1];

    switch (pdu)
    {
    case A_IndividualAddress_Read:
        if (device_prog_mode())
        {
            A_IndividualAddress_Read__ind(priority, hop_count_type);
        }
        break;

    case A_IndividualAddress_Write:
        if (device_prog_mode())
        {
            console_print_string("A_IndividualAddress_Write TOCHECK\r\n");
            uint16_t addr = (tsdu[2] << 8) | (tsdu[3]);
            A_IndividualAddress_Write__ind(priority, hop_count_type, addr);
        }
        break;
    case A_DomainAddress_Read: /* 0x3E1 */
        if (device_prog_mode())
        {
            console_print_string("A_DomainAddress_Read TODO\r\n");
        }
        break;
    default:
        console_print_string("T_Data_Broadcast__ind not managed ");
        console_print_hex(pdu);
        console_print_string("\r\n");
        break;
    }
}
