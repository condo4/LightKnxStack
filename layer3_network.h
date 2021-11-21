#ifndef __LAYER_NETWORK__
#define __LAYER_NETWORK__

#include "knx_types.h"

/* 2.2.1 N_Data_Individual service */

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
                            uint8_t *nsdu);

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
                            uint8_t *nsdu, N_Status n_status);

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
                            uint8_t *nsdu);

/* 2.2.2 N_Data_Group service */

/**
 * \fn N_Data_Group.req(ack_request, destination_address, hop_count_type, octet_count, priority,
 * nsdu) \param ack_request Data Link Layer acknowledge requested or not \param destination_address
 * Group Address of the destination \param hop_count_type hop count 7 or Network Layer Parameter
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data to be transferred by Network Layer
 */
void N_Data_Group__req(uint8_t ack_request, KnxAddress destination_address, uint8_t hop_count_type,
                       uint8_t octet_count, Priority priority, uint8_t *nsdu);

/**
 * \fn N_Data_Group.con(ack_request, destination_address, hop_count_type, octet_count, priority,
 * nsdu, n_status) \param ack_request Data Link Layer acknwoledge requested or not \param
 * destination_address Group Address of the destination \param hop_count_type hop count 7 or Network
 * Layer Parameter \param octet_count length information as described in Data Link Layer \param
 * priority system, urgent, normal or low priority \param nsdu this is the user data that has been
 * transferred by Network Layer \param n_status ok: N_Data_Group sent successfully with L_Data
 * service not_ok: transmission of the associated L_Data request frame did not succeed
 */
void N_Data_Group__con(uint8_t ack_request, KnxAddress destination_address, uint8_t hop_count_type,
                       uint8_t octet_count, Priority priority, uint8_t *nsdu, N_Status n_status);

/**
 * \fn N_Data_Group.ind(destination_address, hop_count_type, octet_count, priority, nsdu)
 * \param destination_address the addressed Group Address of this device
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data that has been transferred by Network Layer
 */
void N_Data_Group__ind(KnxAddress destination_address, uint8_t hop_count_type, uint8_t octet_count,
                       Priority priority, uint8_t *nsdu, KnxAddress source_address);

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
                           Priority priority, uint8_t *nsdu);

/**
 * \fn N_Data_Broadcast.con(ack_request, hop_count_type, octet_count, priority, nsdu, n_status)
 * \param ack_request Data Link Layer acknowledge requested or not
 * \param hop_count_type hop count 7 or Network Layer Parameter
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param nsdu this is the user data that has been transferred by Network Layer
 * \param n_status ok: N_Data_Broadcast sent successfully with L_Data service not_ok: transmission
 * of the associated L_Data request frame did not succeed
 */
void N_Data_Broadcast__con(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *nsdu, N_Status n_status);

/**
 * \fn N_Data_Broadcast.ind(hop_count_type, octet_count, priority, source_address, nsdu)
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param source_address Individual Address of the device that requested the N_Data_Broadcast
 * service \param nsdu this is the user data that has been transferred by Network Layer
 */
void N_Data_Broadcast__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, uint8_t *nsdu);

/* 2.2.4 N_Data_SystemBroadcast service */

/**
 * \fn N_Data_SystemBroadcast.req(ack_request, hop_count_type, nsdu, octet_count, priority)
 * \param ack_request This parameter shall be used to indicate whether a Data Link Layer acknowledge
 * is mandatory or optional. \param hop_count_type This parameter shall be used to indicate whether
 * the hop_count shall be set to 7 or if the Network Layer parameter shall be used. \param nsdu This
 * parameter shall be used to contain the user data that shall be transferred by the Network Layer.
 * \param octet_count This parameter shall be used to indicate the length information of the
 * requested frame. \param priority This parameter shall be used to indicate the priority that shall
 * be used to transmit the requested frame; it shall be “system”, “urgent”, “normal” or “low”.
 */
void N_Data_SystemBroadcast__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t *nsdu,
                                 uint8_t octet_count, Priority priority);

/**
 * \fn N_Data_SystemBroadcast.con(ack_request, hop_count_type, nsdu, octet_count, priority,
 * n_status) \param ack_request This parameter shall be used to indicate whether a Data Link Layer
 * acknowledge is indicated as mandatory or optional in the transmitted frame. \param hop_count_type
 * This parameter shall be used to indicate whether the hop_count of the transmitted frame is set to
 * 7 or if the Network Layer parameter is used. \param nsdu: This parameter shall be used to contain
 * the user data that is transferred by the Network Layer. \param octet_count: This parameter shall
 * be used to indicate the length information of the transmitted frame. \param priority This
 * parameter shall be used to indicate the priority that is used to transmit the requested frame; it
 * shall be “system”, “urgent”, “normal” or “low”. \param n_status ok: This value of this parameter
 * shall be used to indicate that the transmission of the N_Data_SystemBroadcast is successful.
 * not_ok: This value of this parameter shall be used to indicate that the transmission of the
 * N_Data_SystemBroadcast.req did not succeed.
 */
void N_Data_SystemBroadcast__con(uint8_t ack_request, uint8_t hop_count_type, uint8_t *nsdu,
                                 uint8_t octet_count, Priority priority, N_Status n_status);

/**
 * \fn N_Data_SystemBroadcast.ind(hop_count_type, nsdu, octet_count, priority, source_address)
 * \param hop_count_type This parameter shall be used to indicate whether the hop count of the
 * received frame equals 7 or not. \param nsdu This parameter shall be used to contain the user data
 * that is received by the Network Layer. \param octet_count This parameter shall be used to contain
 * the length information of the received frame. \param priority This parameter shall be used to
 * indicate the priority of the received frame; it shall be “system”, “urgent”, “normal” or “low”.
 * \param source_address This parameter shall be used to indicate the Source Address of the received
 * frame; it shall be the Individual Address of the device that has transmitted the
 * N_Data_SystemBroadcast-PDU.
 */
void N_Data_SystemBroadcast__ind(uint8_t hop_count_type, uint8_t *nsdu, uint8_t octet_count,
                                 Priority priority, KnxAddress source_address);

#endif
