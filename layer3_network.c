#include "layer3_network.h"
#include "layer2_data_link.h"
#include "layer4_transport.h"
#include "object_address_table.h"
#include "object_device.h"

#include "debug.h"


static bool _confirmed = false;
/* This layer must be the first layer asynchronious */

/* 2.3 Parameters of Network Layer */
enum DeviceType
{
    DeviceType_Normal,
    DeviceType_Bridge,
    DeviceType_Router
};

/**
 * \fn N_Data_Individual.req(ack_request, destination_address, hop_count_type, octet_count,
 * priority, nsdu) \param ack_request Data Link Layer acknowledge requested or don’t care \param
 * destination_address  Individual Address of the destination \param hop_count_type hop count 7 or
 * Network Layer Parameter \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data to be transferred by the Network Layer
 */
void N_Data_Individual__req(uint8_t ack_request, KnxAddress destination_address,
                            uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                            uint8_t *nsdu)
{
#if defined(DEBUG_LAYER_NETWORK)
    int i;
    console_print_string("N_Data_Individual__req(");
    console_print_int(ack_request);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", [");
    for (i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(nsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(nsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif
    L_Data__req(ack_request, Individual, destination_address, L_Data_Standard, nsdu, octet_count, priority, device_induvidual_address());

    /* Wait confirmation */

    while(!_confirmed)
    {

    }
}

/**
 * \fn N_Data_Individual.con(ack_request, destination_address, hop_count_type, octet_count,
 * priority, nsdu, n_status) \param ack_request Data Link Layer acknowledge requested or don’t care
 * \param destination_address Individual Address of the destination
 * \param hop_count_type hop count 7 or Network Layer Parameter
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data that has been transferred by Network Layer
 * \param n_status ok: N_Data_Individual sent successfully with L_Data not_ok: transmission of the
 * associated L_Data request frame did not succeed
 */
void N_Data_Individual__con(uint8_t ack_request, KnxAddress destination_address,
                            uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                            uint8_t *nsdu, N_Status n_status)
{
#if defined(DEBUG_LAYER_NETWORK)
    console_print_string("N_Data_Individual__con() confirmed");
    console_print_string("])\r\n");
#endif
    _confirmed = true;
}


/**
 * \fn N_Data_Individual.ind(destination_address, hop_count_type, octet_count, priority,
 * source_address, nsdu) \param destination_address the Individual Address of this device \param
 * hop_count_type hop count equals 7 or not \param octet_count length information as described in
 * Data Link Layer \param priority system, urgent, normal or low priority \param source_address
 * Individual Address of the device that requested the N_Data_Individual service \param nsdu this is
 * the user data that has been transferred by Network Layer
 */
void N_Data_Individual__ind(KnxAddress destination_address, uint8_t hop_count_type,
                            uint8_t octet_count, Priority priority, KnxAddress source_address,
                            uint8_t *nsdu)
{
#if defined(DEBUG_LAYER_NETWORK)
    console_print_string("N_Data_Individual__ind(");
    print_dest_indiv_address(destination_address);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(nsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(nsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif

    T_Data_Individual__ind(hop_count_type, octet_count, priority,
                           source_address, nsdu);
}

/**
 * \fn N_Data_Group.ind(destination_address, hop_count_type, octet_count, priority, nsdu)
 * \param destination_address the addressed Group Address of this device
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data that has been transferred by Network Layer
 * \param source_address MISSING IN SPEC BUT NEED
 */
void N_Data_Group__ind(KnxAddress destination_address, uint8_t hop_count_type, uint8_t octet_count,
                       Priority priority, uint8_t *nsdu, KnxAddress source_address)
{
#if defined(DEBUG_LAYER_NETWORK)
    console_print_string("N_Data_Group__ind(");
    print_dest_group_address(destination_address);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(nsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(nsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif

    T_Data_Group__ind(hop_count_type, octet_count, priority, source_address,
                      address_table_get_tsap(destination_address), nsdu);
}

/* 2.2.3 N_Data_Broadcast service */

/**
 * \fn N_Data_Broadcast.req(ack_request, hop_count_type, octet_count, priority, nsdu)
 * \param ack_request Data Link Layer acknowledge requested or not
 * \param hop_count_type hop count 7 or Network Layer Parameter
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data to be transferred by Network Layer
 */
void N_Data_Broadcast__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *nsdu)
{
#if defined(DEBUG_LAYER_NETWORK)
    int i;
    console_print_string("N_Data_Broadcast__req(");
    console_print_int(ack_request);
    console_print_string(", ");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    console_print_bytes(nsdu, octet_count);
    console_print_string(")\r\n");
#endif
    L_Data__req(ack_request, Multicast, 0, L_Data_Standard, nsdu, octet_count, priority,
                device_induvidual_address());
    console_print_string("N_Data_Broadcast__req END OK\r\n");
}

/**
 * \fn N_Data_Broadcast.ind(hop_count_type, octet_count, priority, source_address, nsdu)
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param source_address Individual Address of the device that requested the N_Data_Broadcast
 * service \param nsdu this is the user data that has been transferred by Network Layer
 */
void N_Data_Broadcast__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, uint8_t *nsdu)
{
#if defined(DEBUG_LAYER_NETWORK)

    console_print_string("N_Data_Broadcast__ind(");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(nsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(nsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif
    T_Data_Broadcast__ind(hop_count_type, octet_count, priority, source_address, nsdu);
}

void N_Data_SystemBroadcast__ind(uint8_t hop_count_type, uint8_t *nsdu, uint8_t octet_count,
                                 Priority priority, KnxAddress source_address)
{
#ifdef DEBUG_LAYER_NETWORK
    console_print_string("N_Data_SystemBroadcast__ind(");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(nsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(nsdu[octet_count - 1]);
    console_print_string("], ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(")\n");
#endif
}
