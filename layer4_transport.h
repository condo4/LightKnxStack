#ifndef __LAYER4_TRANSPORT__
#define __LAYER4_TRANSPORT__

#include "knx_types.h"

/* 3.2  T_Data_Group Service */

/**
 * \fn T_Data_Group.req(ack_request, hop_count_type, octet_count, priority, TSAP, tsdu)
 * \param ack_request Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type  hop count 7 or Network Layer Parameter
 * \param octet_count length information as described in Data Link Layer
 * \param priority system, urgent, normal or low priority
 * \param TSAP identifier of the service access point
 * \param tsdu this is the user data to be transferred by Transport Layer
 */
void T_Data_Group__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                       Priority priority, uint8_t TSAP, uint8_t *tsdu);

/**
 * \fn T_Data_Group.con(ack_request, hop_count_type, octet_count, priority, source_address, TSAP,
 * tsdu, t_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * hop_count_type: hop count 7 or Network Layer Parameter \param octet_count: length information as
 * described in Data Link Layer \param priority: system, urgent, normal or low priority \param
 * source_address: the Individual Address of the originator of the message \param TSAP: identifier
 * of the service access point \param tsdu: this is the user data that has been transferred by
 * Transport Layer \param t_status: ok: T_Data_Group sent successfully with N_Data_Group service
 * not_ok: transmission of the associated N_Data_Group request frame didn’t succeed
 */
void T_Data_Group__con(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                       Priority priority, KnxAddress source_address, uint8_t TSAP, uint8_t *tsdu,
                       T_Status t_status);

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
                       KnxAddress source_address, uint8_t TSAP, uint8_t *tsdu);

/* 3.3 T_Data_Tag_Group service */
/**
 * \fn T_Data_Tag_Group.req(ack_request, destination_address, frame_format, hop_count_type,
 * octet_count, priority, tsdu) \param ack_request This parameter shall be used to indicate whether
 * a Layer-2 acknowledge is mandatory or optional. \param destination_address: This parameter shall
 * be used to contain the Destination Address of the receiver. \param frame_format: This parameter
 * shall be used to indicate the frame format that shall be used to handle the service. \param
 * hop_count_type: hop count 7 or Network Layer Parameter \param octet_count: length information as
 * described in Data Link Layer \param priority: system, urgent, normal or low priority \param tsdu:
 * this is the user data to be transferred by Transport Layer
 */
void T_Data_Tag_Group__req(uint8_t ack_request, KnxAddress destination_address,
                           FrameFormat frame_format, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *tsdu);

/**
 * \fn T_Data_Tag_Group.con(hop_count_type, octet_count, priority, source_address,
 * destination_address, frame_format, tsdu, t_status) \param hop_count_type: hop count 7 or Network
 * Layer Parameter \param octet_count: length information as described in Data Link Layer \param
 * priority: system, urgent, normal or low priority \param source_address: the Individual Address of
 * the originator of the message \param destination_address: Destination Address of the receiver
 * \param frame_format: Used Frame Format.
 * \param tsdu: this is the user data that has been transferred by Transport Layer
 * \param t_status:  ok: T_Data_Tag_Group sent successfully with N_Data_Group service not_ok:
 * transmission of the associated N_Data_Group request frame didn’t succeed
 */
void T_Data_Tag_Group__con(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, KnxAddress destination_address,
                           FrameFormat frame_format, uint8_t *tsdu, T_Status t_status);

/**
 * \fn T_Data_Tag_Group.ind(hop_count_type, octet_count, priority, source_address,
 * destination_address, frame_format, tsdu) \param hop_count_type: hop count equals 7 or not \param
 * octet_count: length information as described in Data Link Layer \param priority: system, urgent,
 * normal or low priority \param source_address: the Individual Address of the originator of the
 * message \param destination_address: Destination Address of the receiver \param frame_format: Used
 * Frame Format. \param tsdu: this is the user data that has been transferred by Transport Layer
 */
void T_Data_Tag_Group__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, KnxAddress destination_address,
                           FrameFormat frame_format, uint8_t *tsdu);

/* 3.4 T_Data_Broadcast Service */

/**
 * \fn T_Data_Broadcast.req(ack_request, hop_count_type, octet_count, priority, tsdu)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type: hop count 7 or Network Layer Parameter
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param tsdu: This shall be the user data to be transferred by Transport Layer
 */
void T_Data_Broadcast__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *tsdu);

/**
 * \fn T_Data_Broadcast.con(ack_request, hop_count_type, octet_count, priority, tsdu, t_status)
 * \param ack_request Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type hop count 7 or Network Layer Parameter
 * \param octet_count length information as described in Data Link Layer
 * \param priority highest, urgent, normal or low priority
 * \param tsdu this is the user data that has been transferred by Transport Layer
 * \param t_status ok: T_Data_Broadcast sent successfully with N_Data_Broadcast service not_ok:
 * transmission of the associated N_Data_Broadcast request frame did not succeed
 */
void T_Data_Broadcast__con(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *tsdu, T_Status t_status);

/**
 * \fn T_Data_Broadcast.ind(hop_count_type, octet_count, priority, source_address, tsdu)
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority highest, urgent, normal or low priority
 * \param source_address Individual Address of the device that requested the T_Data_Broadcast
 * service \param tsdu This shall be the user data that has been transferred by Transport Layer.
 */
void T_Data_Broadcast__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, uint8_t *tsdu);

/* 3.5 T_Data_SystemBroadcast */

/**
 * \fn T_Data_SystemBroadcast.req(ack_request, hop_count_type, octet_count, priority, tsdu)
 * \param ack_request: This parameter shall be used to indicate whether a Layer-2 acknowledge is
 * mandatory or optional. \param hop_count_type: This parameter shall be used to indicate whether
 * the hop_count shall be set to 7 or if the Network Layer parameter shall be used. \param
 * octet_count: This parameter shall be used to indicate the length information of the requested
 * frame. \param priority: This parameter shall be used to indicate the priority that shall be used
 * to the transmit the requested frame; it shall be “system”, “urgent”, “normal” or “low”. \param
 * tsdu: This parameter shall be used to contain the user data to be transferred by the Transport
 * Layer.
 */
void T_Data_SystemBroadcast__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                                 Priority priority, uint8_t *tsdu);

/**
 * \fn T_Data_SystemBroadcast.con(hop_count_type, octet_count, priority, tsdu, t_status)
 * \param hop_count_type: This parameter shall be used to indicate whether the hop_count of the
 * transmitted frame is set to 7 or if the Network Layer parameter is used. \param octet_count: This
 * parameter shall be used to indicate the length information of the transmitted frame. \param
 * priority: This parameter shall be used to indicate the priority that is used to the transmit the
 * requested frame; it shall be “system”, “urgent”, “normal” or “low” \param tsdu: This parameter
 * shall contain the user data that is transferred by Transport Layer. \param t_status: ok: This
 * value of this parameter shall be used to indicate that the transmission of the
 * T_Data_SystemBroadcast.req is successful. not_ok: This value of this parameter shall be used to
 * indicate that the transmission of the T_Data_SystemBroadcast.req did not succeed.
 */
void T_Data_SystemBroadcast__con(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                                 uint8_t *tsdu, T_Status t_status);

/**
 * \fn T_Data_SystemBroadcast.ind(hop_count_type, octet_count, priority, source_address, tsdu)
 * \param hop_count_type: This parameter shall be used to indicate whether the hop count of the
 * received frame equals 7 or not. \param octet_count: This parameter shall be used to contain the
 * length information of the received frame. \param priority: This parameter shall be used to
 * indicate the priority of the received frame; it shall be “system”, “urgent”, “normal” or “low”.
 * \param source_address: This parameter shall be used to indicate the Source Address of the
 * received frame; it shall be the Individual Address of the device that has transmitted the
 * T_Data_SystemBroadcast service. \param tsdu: This parameter shall contain the user data that has
 * been transferred by Transport Layer.
 */
void T_Data_SystemBroadcast__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                                 KnxAddress source_address, uint8_t *tsdu);

/* 3.6 T_Data_Individual */

/**
 * \fn T_Data_Individual.req(ack_request, hop_count_type, octet_count, priority, TSAP, tsdu)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type: hop count 7 or Network Layer Parameter
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point (may be direct the Individual Address of the
 * remote partner) \param tsdu: is the user data to be transferred by Transport Layer
 */
void T_Data_Individual__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                            Priority priority, uint8_t TSAP, uint8_t *tsdu);

/**
 * \fn T_Data_Individual.con(ack_request, hop_count_type, octet_count, priority, TSAP, tsdu,
 * t_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * hop_count_type: hop count 7 or Network Layer Parameter \param octet_count: length information as
 * described in Data Link Layer \param priority: highest, urgent, normal or low priority \param
 * TSAP: identifier of the service access point (may be direct the Individual Address of the remote
 * partner) \param tsdu: this is the user data that has been transferred by Transport Layer \param
 * t_status: ok: T_Data_Individual sent successfully with N_Data_Individual service not_ok:
 * transmission of the associated N_Data_Individual request frame did not succeed
 */
void T_Data_Individual__con(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                            Priority priority, uint8_t TSAP, uint8_t *tsdu, T_Status t_status);

/**
 * \fn T_Data_Individual.ind(hop_count_type, octet_count, priority, TSAP, tsdu)
 * \param hop_count_type: hop count equals 7 or not
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point (may be direct the Individual Address of the
 * remote partner) \param tsdu: is the user data that has been transferred by Transport Layer
 */
void T_Data_Individual__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                            KnxAddress source_address, uint8_t *tsdu);

/* 3.7 T_Connect Service */

/**
 * \fn T_Connect.req (destination_address, priority)
 * \param destination_address: Indivudual Address of the device to which the transport connection
 * shall be established. \param priority: highest, urgent, normal or low priority
 */
void T_Connect__req(KnxAddress destination_address, Priority priority);

/**
 * \fn T_Connect.con (destination_address, TSAP, t_status)
 * \param destination_address: Individual Address of the device to which the transport connection
 * was requested \param TSAP: identifier of the service access point (may be direct the Individual
 * Address of the remote partner) \param t_status: ok: T_Connect.req sent successfully with
 * N_Data_Individual service not_ok: transmission of the associated N_Data_Individual request frame
 * did not succeed
 */
void T_Connect__con(KnxAddress destination_address, uint8_t TSAP, T_Status t_status);

/**
 * \fn T_Connect.ind(TSAP)
 * \param TSAP: identifier of the service access point (may be direct the Individual Address of the
 * remote partner)
 */
void T_Connect__ind(uint8_t TSAP);

/* 3.8 T_Disconnect Service */

/**
 * \fn T_Disconnect.req(priority, TSAP)
 * \param priority highest, urgent, normal or low priority
 * \param TSAP identifier of the service access point to which the connection shall be released
 */
void T_Disconnect__req(Priority priority, uint8_t TSAP);

/**
 * \fn T_Disconnect.con(priority, TSAP, t_status)
 * \param priority highest, urgent, normal or low priority
 * \param TSAP identifier of the service access point to which the release of the connection was
 * requested \param t_status ok: T_Disconnect.req sent successfully with N_Data_Individual service
 * not_ok: The transmission of the associated N_Data_Individual-frame did not succeed
 */
void T_Disconnect__con(Priority priority, uint8_t TSAP, T_Status t_status);

/**
 * \fn T_Disconnect.ind(TSAP)
 * \param TSAP: Identifier of the service access point to which the connection is released.
 */
void T_Disconnect__ind(uint8_t TSAP);

/* 3.9 T_Data_Connected Service */

/**
 * \fn T_Data_Connected.req (octet_count, priority, TSAP, tsdu)
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point to which the frame shall be sent
 * \param tsdu: this is the user data to be transferred by Transport Layer
 */
void T_Data_Connected__req(uint8_t octet_count, Priority priority, uint8_t TSAP, uint8_t *tsdu);

/**
 * \fn T_Data_Connected.con(TSAP)
 * \param TSAP: identifier of the service access point to which the frame has been sent
 */
void T_Data_Connected__con(uint8_t TSAP);

/**
 * \fn T_Data_Connected.ind(octet_count, priority, TSAP, tsdu)
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point from which the frame is received
 * \param tsdu: this is the user data that has been transferred by Transport Layer
 */
void T_Data_Connected__ind(uint8_t octet_count, Priority priority, uint8_t TSAP, uint8_t *tsdu);

#endif
