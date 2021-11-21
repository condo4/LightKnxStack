#ifndef __LAYER2_DATA_LINK__
#define __LAYER2_DATA_LINK__

#include "knx_types.h"

/* 2.2 L_Data service */

/**
 * \fn L_Data.req(ack_request,address_type,destination_address, frame_format, lsdu, octet_count,
 * priority, source_address) \param ack_request This parameter shall be used to indicate whether a
 * Layer-2 acknowledge is mandatory or optional. \param address_type: This parameter shall be used
 * to indicate whether the Destination Address is an Individual Address or a Group Address. \param
 * destination_address: This parameter shall be used to indicate the Destination Address of the
 * Frame to be transmitted; it shall be either an Individual Address or a Group Address. \param
 * frame_format: This parameter shall be used to indicate whether the Frame to be transmitted shall
 * be a standard Frame or an extended Frame. \param lsdu: This parameter shall be used to contain
 * the user data to be transferred by Layer-2. \param octet_count: This parameter shall be used to
 * indicate the length information of the requested Frame. \param priority: This parameter shall be
 * used to indicate the priority that shall be used to the transmit the requested Frame; it shall be
 * “system”, “urgent”, “normal” or “low”. \param source_address Individual Address of the device
 * that requests the L_Data-service.
 */
void L_Data__req(uint8_t ack_request, AddressType address_type, KnxAddress destination_address,
                 FrameFormat frame_format, uint8_t *lsdu, uint8_t octet_count, Priority priority,
                 KnxAddress source_address);

/**
 * \fn L_Data.con(address_type, destination_address, frame_format, octet_count, priority,
 * source_address, lsdu, l_status) \param address_type: This parameter shall indicate whether
 * destination_address is an Individual Address or a Group Address. \param destination_address: This
 * parameter shall be used to indicate the Destination Address of the requested Frame; it shall
 * either be an Individual Address or a Group Address. \param frame_format: This parameter shall be
 * used to indicate whether the transmitted Frame is a standard Frame or an extended Frame. \param
 * octet_count: This parameter shall be used to indicate the length information of the transmitted
 * Frame. \param priority: This parameter shall be used to indicate the priority that is used to
 * transmit the requested Frame; it shall be “system”, “urgent”, “normal” or “low”. \param
 * source_address This parameter shall be used to indicate the Source Address of the requested
 * Frame; it shall be the Individual Address of the device that requests the service primitive.
 * \param lsdu: This parameter shall be the user data transferred by Layer-2.
 * \param l_status: ok: this value of this parameter shall be used to indicate that the transmission
 * of the LSDU has been successful not_ok:  this value of this parameter shall be used to indicate
 * that the transmission of the LSDU did not succeed
 */
void L_Data__con(AddressType address_type, KnxAddress destination_address, FrameFormat frame_format,
                 uint8_t octet_count, Priority priority, KnxAddress source_address, uint8_t *lsdu,
                 L_Status l_status);

/**
 * \fn L_Data.ind(ack_request, address_type,destination_address, frame_format, lsdu, octet_count,
 * priority, source_address) \param ack_request: This parameter shall be used to indicate whether a
 * Layer-2 acknowledge is mandatory or optional. \param address_type: This parameter shall indicate
 * whether destination_address is an Individual Address or a Group Address. \param
 * destination_address: This parameter shall be used to indicate the Destination Address of this
 * Frame. It shall be the Individual Address of this device or a Group Address of this device.
 * \param frame_format: This parameter shall be used to indicate whether the received Frame is a
 * standard Frame or an extended Frame. \param lsdu: This parameter shall be used to contain the
 * user data that is received by Layer-2. \param octet_count: This parameter shall be used to
 * indicate the length information of the received Frame. \param priority: This parameter shall be
 * used to indicate the priority of the received Frame; it shall be “system”, “urgent”, “normal” or
 * “low”. \param source_address: This parameter shall be used to indicate the Source Address of the
 * received Frame; it shall be the Individual Address of the device that has transmitted the Frame.
 */
void L_Data__ind(uint8_t ack_request, AddressType address_type, KnxAddress destination_address,
                 FrameFormat frame_format, uint8_t *lsdu, uint8_t octet_count, Priority priority,
                 KnxAddress source_address);

/* 2.3 L_SystemBroadcast */

/**
 * \fn L_SystemBroadcast.req(ack_request, address_type, destination_address, frame_format, lsdu,
 * octet_count, priority) \param ack_request This parameter shall be used to indicate whether a
 * Layer-2 acknowledge is mandatory or optional. \param address_type This parameter shall be set to
 * “Group Address” \param destination_address This parameter shall be used to indicate the
 * Destination Address of the Frame to be transmitted; it shall be the system broadcast address
 * 0000h \param frame_format This parameter shall be used to indicate whether the Frame to be
 * transmitted shall be a standard Frame or an extended Frame. \param lsdu This parameter shall be
 * used to contain the user data to be transferred by Layer-2. \param octet_count This parameter
 * shall be used to indicate the length information of the requested Frame. \param priority This
 * parameter shall be used to indicate the priority that shall be used to the transmit the requested
 * Frame; it shall be “system”, “urgent”, “normal” or “low”.
 */
void L_SystemBroadcast__req(uint8_t ack_request, AddressType address_type,
                            KnxAddress destination_address, FrameFormat frame_format, uint8_t *lsdu,
                            uint8_t octet_count, Priority priority);

/**
 * \fn L_SystemBroadcast.con(address_type,destination_address, frame_format, octet_count, priority,
 * source_address, lsdu, l_status) \param address_type This parameter shall be set to “Group
 * Address”. \param destination_address This parameter shall be used to indicate the Destination
 * Address of the transmitted Frame; it shall be the system broadcast address 0000h \param
 * frame_format This parameter shall be used to indicate whether the transmitted Frame is a standard
 * Frame or an extended Frame. \param octet_count This parameter shall be used to indicate the
 * length information of the transmitted Frame. \param priority This parameter shall be used to
 * indicate the priority that is used to transmit the requested Frame; it shall be “system”,
 * “urgent”, “normal” or “low”. \param source_address This parameter shall be used to indicate the
 * Source Address of the requested Frame; it shall be the Individual Address of the device that
 * requests the service primitive. \param lsdu This parameter shall be the user data transferred by
 * Layer-2 \param l_status ok: this value of this parameter shall be used to indicate that the
 * transmission of the LSDU has been successful not_ok:  this value of this parameter shall be used
 * to indicate that the transmission of the LSDU did not succeed
 */
void L_SystemBroadcast__con(AddressType address_type, KnxAddress destination_address,
                            FrameFormat frame_format, uint8_t octet_count, Priority priority,
                            KnxAddress source_address, uint8_t *lsdu, L_Status l_status);

/**
 * \fn L_SystemBroadcast.ind(ack_request, address_type, destination_address, lsdu, octet_count,
 * priority, source_address) \param ack_request This parameter shall be used to indicate whether a
 * Layer-2 acknowledge is mandatory or optional. \param address_type This parameter shall be set to
 * “Group Address”. \param destination_address This parameter shall be used to indicate the
 * Destination Address of the received Frame; it shall be the system broadcast address 0000h \param
 * frame_format This parameter shall be used to indicate whether the received Frame is a standard
 * Frame or an extended Frame. \param lsdu This parameter shall be used to contain the user data
 * that is received by Layer-2. \param octet_count This parameter shall be used to indicate the
 * length information of the received Frame. \param priority This parameter shall be used to
 * indicate the priority of the received Frame; it shall be “system”, “urgent”, “normal” or “low”.
 * \param source_address This parameter shall be used to indicate the Source Address of the received
 * Frame; it shall be the Individual Address of the device that has transmitted the Frame.
 */
void L_SystemBroadcast__ind(uint8_t ack_request, AddressType address_type,
                            KnxAddress destination_address, uint8_t *lsdu, uint8_t octet_count,
                            Priority priority, KnxAddress source_address);

void L_Loop(void);

#endif
