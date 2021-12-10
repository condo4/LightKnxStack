#ifndef __LAYER7_APPLICATION__
#define __LAYER7_APPLICATION__

#include "knx_types.h"

/* 2 APDU */
#define A_GroupValue_Read (0x000)
#define A_GroupValue_Response (0x040)
#define A_GroupValue_Write (0x080)
#define A_IndividualAddress_Write (0x0C0)
#define A_IndividualAddress_Read (0x100)
#define A_IndividualAddress_Response (0x140)
#define A_ADC_Read (0x180)
#define A_ADC_Response (0x1C0)
#define A_SystemNetworkParameter_Read (0x1C8)
#define A_SystemNetworkParameter_Response (0x1C9)
#define A_SystemNetworkParameter_Write (0x1CA)
#define A_Memory_Read (0x200)
#define A_Memory_Response (0x240)
#define A_Memory_Write (0x280)
#define A_UserMemory_Read (0x2C0)
#define A_UserMemory_Response (0x2C1)
#define A_UserMemory_Write (0x2C2)
#define A_UserManufacturerInfo_Read (0x2C5)
#define A_UserManufacturerInfo_Response (0x2C6)
#define A_FunctionPropertyCommand (0x2C7)
#define A_FunctionPropertyState_Read (0x2C8)
#define A_FunctionPropertyState_Response (0x2C9)
#define A_DeviceDescriptor_Read (0x300)
#define A_DeviceDescriptor_Response (0x340)
#define A_Restart (0x380)
#define A_Authorize_Request (0x3D1)
#define A_Authorize_Response (0x3D2)
#define A_Key_Write (0x3D3)
#define A_Key_Response (0x3D4)
#define A_PropertyValue_Read (0x3D5)
#define A_PropertyValue_Response (0x3D6)
#define A_PropertyValue_Write (0x3D7)
#define A_PropertyDescription_Read (0x3D8)
#define A_PropertyDescription_Response (0x3D9)
#define A_NetworkParameter_Read (0x3DA)
#define A_NetworkParameter_Response (0x3DB)
#define A_IndividualAddressSerialNumber_Read (0x3DC)
#define A_IndividualAddressSerialNumber_Response (0x3DD)
#define A_IndividualAddressSerialNumber_Write (0x3DE)
#define A_DomainAddress_Write (0x3E0)
#define A_DomainAddress_Read (0x3E1)
#define A_DomainAddress_Response (0x3E2)
#define A_DomainAddressSelective_Read (0x3E3)
#define A_NetworkParameter_Write (0x3E4)
#define A_Link_Read (0x3E5)
#define A_Link_Response (0x3E6)
#define A_Link_Write (0x3E7)
#define A_FileStream_InfoReport (0x3F0)

/* 3.1   Application Layer services on Multicast Communication Mode */

/* 3.1.2 A_GroupValue_Read-service */

/**
 * \fn A_GroupValue_Read.req(ack_request, ASAP, priority, hop_count_type)
 * \param ack_request Data Link Layer Acknowledge requested or don’t care
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 */
void A_GroupValue_Read__req(uint8_t ack_request, uint8_t ASAP, Priority priority,
                            uint8_t hop_count_type);

/**
 * \fn A_GroupValue_Read.Lcon(ack_request ,ASAP, priority, hop_count_type, a_status)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 * \param a_status: ok: A_GroupValue_Read.req sent successfully with T_Data_Group service not_ok:
 * transmission of the associated T_Data_Group request frame did not succeed
 */
void A_GroupValue_Read__Lcon(uint8_t ack_request, uint8_t ASAP, Priority priority,
                             uint8_t hop_count_type, A_Status a_status);

/**
 * \fn A_GroupValue_Read.ind(ASAP, priority, hop_count_type)
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 */
void A_GroupValue_Read__ind(uint8_t ASAP, Priority priority, uint8_t hop_count_type);

/**
 * \fn A_GroupValue_Read.res(ack_request, ASAP, priority, hop_count_type, data)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 * \param data: the value of the associated Service Access Point
 */
void A_GroupValue_Read__res(uint8_t ack_request, uint8_t ASAP, Priority priority,
                            uint8_t hop_count_type, uint8_t *data);

/**
 * \fn A_GroupValue_Read.Rcon(ack_request, ASAP, priority, hop_count_type, data, a_status)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 * \param data: the value of the associated Service Access Point
 * \param a_status: ok: A_GroupValue_Read.res sent successfully with T_Data_Group service not_ok:
 * transmission of the associated T_Data_Group request frame did not succeed
 */
void A_GroupValue_Read__Rcon(uint8_t ack_request, uint8_t ASAP, Priority priority,
                             uint8_t hop_count_type, uint8_t *data, A_Status a_status);

/**
 * \fn A_GroupValue_Read.Acon(ASAP, priority, hop_count_type, data)
 * \param ASAP: local reference of the Service Access Point
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 * \param data: the value of the associated Service Access Point
 * \param octet_count: Need for check and debug only
 */
void A_GroupValue_Read__Acon(uint8_t ASAP, Priority priority, uint8_t hop_count_type, uint8_t *data,
                             uint8_t octet_count);

/* 3.1.3 A_GroupValue_Write-service */

/**
 * \fn A_GroupValue_Write.req(ack_request, ASAP, priority, hop_count_type, data)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param ASAP: local reference of the Service Access Point
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param data: data of the associated application layer Service Access Point
 */
void A_GroupValue_Write__req(uint8_t ack_request, uint8_t ASAP, Priority priority,
                             uint8_t hop_count_type, uint8_t *data);

/**
 * \fn A_GroupValue_Write.Lcon(ack_request, ASAP, priority, hop_count_type, data, a_status)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param ASAP: local reference of the Service Access Point
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param data: data of the associated application layer Service Access Point
 * \param a_status: ok:  A_GroupValue_Write sent successfully with T_Data_Group service not_ok:
 * transmission of the associated T_Data_Group request frame did not succeed
 */
void A_GroupValue_Write__Lcon(uint8_t ack_request, uint8_t ASAP, Priority priority,
                              uint8_t hop_count_type, uint8_t *data, A_Status a_status);

/**
 * \fn A_GroupValue_Write.ind(ASAP, priority, hop_count_type, data)
 * \param ASAP: local reference of the Service Access Point
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param data: data of the associated application layer Service Access Point
 * \param octet_count: Need for check and debug only
 */
void A_GroupValue_Write__ind(uint8_t ASAP, Priority priority, uint8_t hop_count_type, uint8_t *data,
                             uint8_t octet_count);

/* 3.2 Application Layer services on Broadcast Communication Mode */

/* 3.2.2 A_IndividualAddress_Write-service */

/**
 * \fn A_IndividualAddress_Write.req(ack_request, priority, hop_count_type, newaddress)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param newaddress: the new value of the Individual Address
 */
void A_IndividualAddress_Write__req(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                    KnxAddress newaddress);

/**
 * \fn A_IndividualAddress_Write.Lcon(ack_request, priority, hop_count_type, newaddress, a_status)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param newaddress: the new value of the Individual Address
 * \param a_status: ok: A_IndividualAddress_Write sent successfully with T_Data_Broadcast service
 * not_ok: transmission of the associated T_Data_Broadcast request frame did not succeed
 */
void A_IndividualAddress_Write__Lcon(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                     KnxAddress newaddress, A_Status a_status);

/**
 * \fn A_IndividualAddress_Write.ind(priority, hop_count_type, newaddress)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param newaddress: the new value of the Individual Address
 */
void A_IndividualAddress_Write__ind(Priority priority, uint8_t hop_count_type,
                                    KnxAddress newaddress);

/* 3.2.3 A_IndividualAddress_Read-service */

/**
 * \fn A_IndividualAddress_Read.req(ack_request, priority, hop_count_type)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 */
void A_IndividualAddress_Read__req(uint8_t ack_request, Priority priority, uint8_t hop_count_type);

/**
 * \fn A_IndividualAddress_Read.Lcon(ack_request, priority, hop_count_type, a_status)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param a_status: ok: A_IndividualAddress_Read sent successfully with T_Data_Broadcast service
 * not_ok: transmission of the associated T_Data_Broadcast request frame did not succeed
 */
void A_IndividualAddress_Read__Lcon(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                    A_Status a_status);

/**
 * \fn A_IndividualAddress_Read.ind(priority, hop_count_type)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 */
void A_IndividualAddress_Read__ind(Priority priority, uint8_t hop_count_type);

/**
 * \fn A_IndividualAddress_Read.res(ack_request, priority, hop_count_type, Individual Address)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param individual_address: the value of the Individual Address
 */
void A_IndividualAddress_Read__res(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                   KnxAddress individual_address);

/**
 * \fn A_IndividualAddress_Read.Rcon(ack_request, priority, hop_count_type, individual_address,
 * a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * priority: system, urgent, normal or low priority \param hop_count_type: hop count 7 or standard
 * \param individual_address: the value of the Individual Address
 * \param a_status: ok: A_IndividualAddress_Read sent successfully with T_Data_Broadcast service
 * not_ok: transmission of the associated T_Data_Broadcast request frame did not succeed
 */
void A_IndividualAddress_Read__Rcon(uint8_t ack_request, Priority priority, uint8_t hop_count_type,
                                    KnxAddress individual_address, A_Status a_status);

/**
 * \fn A_IndividualAddress_Read.Acon(priority, hop_count_type, Individual Address)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param individual_address: the value of the Individual Address
 */
void A_IndividualAddress_Read__Acon(Priority priority, uint8_t hop_count_type,
                                    KnxAddress individual_address);

/* 3.2.4 A_IndividualAddressSerialNumber_Read-service */

/**
 * \fn A_IndividualAddressSerialNumber_Read.req (ack_request, priority, hop_count_type,
 * serial_number) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * priority: system, urgent, normal or low priority \param hop_count_type: hop count 7 or standard
 * \param serial_number: the KNX Serial Number
 */
void A_IndividualAddressSerialNumber_Read__req(uint8_t ack_request, Priority priority,
                                               uint8_t hop_count_type, uint8_t *serial_number);

/**
 * \fn A_IndividualAddressSerialNumber_Read.Lcon (ack_request, priority, hop_count_type,
 * serial_number,a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param serial_number: the KNX Serial Number
 * \param a_status ok: A_IndividualAddressSerialNumber_Read sent successfully with T_Data_Broadcast
 * service not_ok: transmission of the associated T_Data_Broadcast request frame did not succeed
 */
void A_IndividualAddressSerialNumber_Read__Lcon(uint8_t ack_request, Priority priority,
                                                uint8_t hop_count_type, uint8_t *serial_number,
                                                A_Status a_status);

/**
 * \fn A_IndividualAddressSerialNumber_Read.ind (priority, hop_count_type, serial_number)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param serial_number: the KNX Serial Number
 */
void A_IndividualAddressSerialNumber_Read__ind(Priority priority, uint8_t hop_count_type,
                                               uint8_t *serial_number);

/**
 * \fn A_IndividualAddressSerialNumber_Read.res (ack_request, priority, hop_count_type,
 * serial_number,domain_address) \param ack_request: Data Link Layer Acknowledge requested or don’t
 * care \param priority: system, urgent, normal or low priority \param hop_count_type: hop count 7
 * or standard \param serial_number: the KNX Serial Number \param domain_address: the Domain Address
 * of the remote device
 */
void A_IndividualAddressSerialNumber_Read__res(uint8_t ack_request, Priority priority,
                                               uint8_t hop_count_type, uint8_t *serial_number,
                                               KnxAddress domain_address);

/**
 * \fn A_IndividualAddressSerialNumber_Read.Rcon (ack_request, priority, hop_count_type,
 * serial_number,a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param serial_number: the KNX Serial Number
 * \param a_status ok: A_IndividualAddressSerialNumber_Response sent successfully with
 * T_Data_Broadcast service not_ok: transmission of the associated T_Data_Broadcast request frame
 * did not succeed
 */
void A_IndividualAddressSerialNumber_Read__Rcon(uint8_t ack_request, Priority priority,
                                                uint8_t hop_count_type, uint8_t *serial_number,
                                                A_Status a_status);

/**
 * \fn A_IndividualAddressSerialNumber_Read.Acon (priority, hop_count_type,
 * serial_number,individual_address, domain_address) \param priority: system, urgent, normal or low
 * priority \param hop_count_type: hop count 7 or standard \param serial_number: the KNX Serial
 * Number \param individual_address: the value of the Individual Address \param domain_address: the
 * Domain Address of the remote device
 */
void A_IndividualAddressSerialNumber_Read__Acon(Priority priority, uint8_t hop_count_type,
                                                uint8_t *serial_number,
                                                KnxAddress individual_address,
                                                KnxAddress domain_address);

/* 3.2.5 A_IndividualAddressSerialNumber_Write-service */

/**
 * \fn A_IndividualAddressSerialNumber_Write.req(ack_request, priority, hop_count_type,
 * serial_number, newaddress) \param ack_request: Data Link Layer Acknowledge requested or don’t
 * care \param priority: system, urgent, normal or low priority \param hop_count_type: hop count 7
 * or standard \param serial_number: the serial number \param newaddress: the new value of the
 * Individual Address
 */
void A_IndividualAddressSerialNumber_Write__req(uint8_t ack_request, Priority priority,
                                                uint8_t hop_count_type, uint8_t *serial_number,
                                                KnxAddress newaddress);

/**
 * \fn A_IndividualAddressSerialNumber_Write.Lcon(ack_request, priority, hop_count_type, newaddress,
 * a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * priority: system, urgent, normal or low priority \param hop_count_type: hop count 7 or standard
 * \param newaddress: the new value of the Individual Address
 * \param a_status: ok: A_IndividualAddressSerialNumber_Write sent successfully with
 * T_Data_Broadcast service not_ok: transmission of the associated T_Data_Broadcast request frame
 * did not succeed
 */
void A_IndividualAddressSerialNumber_Write__Lcon(uint8_t ack_request, Priority priority,
                                                 uint8_t hop_count_type, KnxAddress newaddress,
                                                 A_Status a_status);

/**
 * \fn A_IndividualAddressSerialNumber_Write.ind(priority, hop_count_type, serial_number,
 * newaddress) \param priority: system, urgent, normal or low priority \param hop_count_type: hop
 * count 7 or standard \param serial_number: the serial number \param newaddress: the new value of
 * the Individual Address
 */
void A_IndividualAddressSerialNumber_Write__ind(Priority priority, uint8_t hop_count_type,
                                                uint8_t *serial_number, KnxAddress newaddress);

/* 3.2.6 A_NetworkParameter_Read-service */

/**
 * \fn A_NetworkParameter_Read.req(ASAP, comm_mode_req, hop_count_type, parameter_type,
 * priority,test_info) \param ASAP: local reference of the Service Access Point or Individual
 * Address \param comm_mode_req: point-to-all-points connectionless communication mode or
 * point-to-point connectionless communication mode \param hop_count_type: hop count 7 or standard
 * \param parameter_type: network parameter type that is verified, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * test_info: value against which the resource indicated by parameter_type is tested
 */

/**
 * \fn A_NetworkParameter_Read.Lcon(ASAP, comm_mode_req, hop_count_type, parameter_type, priority,
 * test_info, a_status) \param ASAP: local reference of the Service Access Point or Individual
 * Address \param comm_mode_req: point-to-all-points connectionless communication mode or
 * point-to-point connectionless communication mode \param hop_count_type: hop count 7 or standard
 * \param parameter_type: network parameter type that is verified, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * test_info value against which the resource indicated by parameter_type is tested \param a_status:
 * ok: A_NetworkParameter_Read.req sent successfully with T_Data_Broadcast service not_ok:
 * transmission of the associated T_Data_Broadcast request frame did not succeed
 */

/**
 * \fn A_NetworkParameter_Read.ind(ASAP, comm_mode_req, hop_count_type, parameter_type, priority,
 * test_info) \param ASAP: local reference of the Service Access Point or individual address \param
 * comm_mode_req: point-to-all-points connectionless communication mode or point-to-point
 * connectionless communication mode \param hop_count_type: hop count 7 or standard \param
 * parameter_type: network parameter type that is verified, structured as Interface Object Type and
 * Property Identifier \param priority: system, urgent, normal or low priority \param test_info:
 * value against which the resource indicated by parameter_type is tested
 */

/**
 * \fn A_NetworkParameter_Read.res(ASAP, comm_mode, hop_count_type, parameter_type, priority,
 * test_info, test_result) \param ASAP: local reference of the Service Access Point or individual
 * address \param comm_mode: point-to-all-points connectionless communication mode or point-to-point
 * connectionless communication mode \param hop_count_type: hop count 7 or standard \param
 * individual_address: the destination address for the service \param parameter_type: network
 * parameter type that is verified, structured as Interface Object Type and Property Identifier
 * \param priority: system, urgent, normal or low priority
 * \param test_info: value against which the resource indicated by parameter_type is tested
 * \param test_result: parameter_type dependent response
 */

/**
 * \fn A_NetworkParameter_Read.Rcon(ASAP, comm_mode, hop_count_type, parameter_type, priority,
 * test_info, test_result, a_status) \param ASAP: local reference of the Service Access Point or
 * individual address \param comm_mode: point-to-all-points connectionless communication mode or
 * point-to-point connectionless communication mode \param hop_count_type: hop count 7 or standard
 * \param individual_address: the destination address for the service
 * \param parameter_type: network parameter type that is verified, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * test_info: value against which the resource indicated by parameter_type is tested \param
 * test_result: parameter_type dependent response \param a_status: ok: A_NetworkParameter_Read.res
 * sent successfully with T_Data_Broadcast or T_Data_Individual service not_ok: transmission of the
 * associated T_Data_Broadcast or T_Data_Individual request frame did not succeed
 */

/**
 * \fn A_NetworkParameter_Read.Acon(ASAP, hop_count_type, individual_address, parameter_type,
 * priority, test_info, test_result) \param ASAP: local reference of the Service Access Point or
 * individual address \param hop_count_type: hop count 7 or standard \param individual_address: the
 * individual address of the device that has sent the response \param parameter_type: network
 * parameter type that is verified, structured as Interface Object Type and Property Identifier
 * \param priority: system, urgent, normal or low priority
 * \param test_info: value against which the resource indicated by parameter_type is tested
 * \param test_result: parameter_type dependent response
 */
/*
void A_NetworkParameter_Read__Acon(
    ASAP,
    uint8_t hop_count_type,
    KnxAddress individual_address,
    parameter_type,
    Priority priority,
    test_info,
    test_result
);
*/

/* 3.2.7 A_NetworkParameter_Write-service */

/**
 * \fn A_NetworkParameter_Write.req(ASAP, comm_mode, hop_count_type, parameter_type, priority,
 * value) \param ASAP: local reference of the Service Access Point or Individual Address \param
 * comm_mode: point-to-all-points connectionless communication mode or point-to-point connectionless
 * communication mode \param hop_count_type: hop count 7 or standard \param parameter_type: the
 * network parameter that shall be set, structured as Interface Object Type and Property Identifier
 * \param priority: system, urgent, normal or low priority
 * \param value: value to which the network parameter indicated by parameter_type shall be set
 */

/**
 * \fn A_NetworkParameter_Write.Lcon(ASAP, comm_mode, hop_count_type, parameter_type, priority,
 * value, a_status) \param ASAP: local reference of the Service Access Point or Individual Address
 * \param comm_mode: point-to-all-points connectionless communication mode or point-to-point
 * connectionless communication mode \param hop_count_type: hop count 7 or standard \param
 * parameter_type: the network parameter that shall be set, structured as Interface Object Type and
 * Property Identifier \param priority: system, urgent, normal or low priority \param value: value
 * to which the network parameter indicated by parameter_type shall be set \param a_status: ok:
 * A_NetworkParameter_Write-PDU sent successfully with the requested Transport Layer service not_ok:
 * transmission of the requested Transport Layer service did not succeed
 */

/**
 * \fn A_NetworkParameter_Write.ind(ASAP, parameter_type, priority, value)
 * \param ASAP: local reference of the Service Access Point or Individual Address
 * \param parameter_type: the network parameter that shall be set, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * value: value to which the network parameter indicated by parameter_type shall be set
 */

/* 3.2.8 A_NetworkParameter_InfoReport */

/**
 * \fn A_NetworkParameter_InfoReport.req(ASAP, comm_mode_req, hop_count_type, parameter_type,
 * priority, test_info, test_result) \param ASAP: local reference of the Service Access Point or
 * Individual Address \param comm_mode_req: point-to-all-points connectionless communication mode or
 * point-to-point connectionless communication mode \param hop_count_type: hop count 0, 7 or Network
 * Layer Parameter \param parameter_type: the network parameter that shall be set, structured as
 * Interface Object Type and Property Identifier \param priority: system, urgent, normal or low
 * priority \param test_info: parameter type dependent value \param test_result: parameter type
 * dependent value
 */

/**
 * \fn A_NetworkParameter_InfoReport.Lcon(ASAP, comm_mode_req, hop_count_type, parameter_type,
 * priority, test_info, test_result, a_status) \param ASAP: local reference of the Service Access
 * Point or Individual Address \param comm_mode_req point-to-all-points connectionless communication
 * mode or point-to-point connectionless communication mode \param hop_count_type: hop count 0, 7 or
 * Network Layer Parameter \param parameter_type: the network parameter that shall be set,
 * structured as Interface Object Type and Property Identifier \param priority: system, urgent,
 * normal or low priority \param test_info: parameter type dependent value \param test_result
 * parameter type dependent value \param a_status: ok: A_NetworkParameter_InfoReport sent
 * successfully with T_Data_Individual - or T_Data_Broadcast service not_ok: transmission of the
 * associated T_Data_Individual – or T_Data_Broadcast request frame did not succeed
 */

/**
 * \fn A_NetworkParameter_InfoReport.ind(ASAP, comm_mode_req, hop_count_type, parameter_type,
 * priority, test_info, test_result) \param ASAP: the Individual Address of the device that has sent
 * the A_NetworkParameter_InfoReport-PDU. \param comm_mode_req point-to-all-points connectionless
 * communication mode or point-to-point connectionless communication mode \param hop_count_type: hop
 * count 0, 7 or Network Layer Parameter \param parameter_type: the network parameter that shall be
 * set, structured as Interface Object Type and Property Identifier \param priority: system, urgent,
 * normal or low priority \param test_info: parameter type dependent value \param test_result:
 * parameter type dependent value
 */

/* 3.3 Application Layer Services on System Broadcast communication mode */

/* 3.3.2 A_DeviceDescriptor_InfoReport-service */

/**
 * \fn A_DeviceDescriptor_InfoReport.req(ack_request, descriptor_type, device_descriptor,
 * hop_count_type, priority) \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param descriptor_type: type of the device descriptor
 * \param device_descriptor: the device descriptor of the device
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system, urgent, normal or low priority
 */

/**
 * \fn A_DeviceDescriptor_InfoReport.Lcon(ack_request, descriptor_type, device_descriptor,
 * hop_count_type, priority, a_status) \param ack_request: Data Link Layer Acknowledge requested or
 * don’t care \param descriptor_type: type of the descriptor \param device_descriptor: the device
 * descriptor of the device \param hop_count_type: hop count 7 or standard \param priority: system,
 * urgent, normal or low priority \param a_status:  ok: A_DeviceDescriptor_InfoReport sent
 * successfully with T_Data_SystemBroadcast service not_ok:  transmission of the associated
 * T_Data_SystemBroadcast request frame did notsucceed
 */

/**
 * \fn A_DeviceDescriptor_InfoReport.ind(descriptor_type device_descriptor, hop_count_type,
 * priority) \param descriptor_type: type of the descriptor \param device_descriptor: the device
 * descriptor of the device \param hop_count_type: hop count 7 or standard \param priority: system,
 * urgent, normal or low priority
 */

/* 3.3.3 A_DomainAddress_Write-service */

/**
 * \fn A_DomainAddress_Write.req(ack_request, priority, hop_count_type, domain_address_new)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param domain_address_new: the new value of the Domain Address
 */

/**
 * \fn A_DomainAddress_Write.Lcon(ack_request, priority, hop_count_type, domain_address_new,
 * a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * priority: system, urgent, normal or low priority \param hop_count_type: hop count 7 or standard
 * \param domain_address_new: the new value of the Domain Address
 * \param a_status: ok: A_DomainAddress_Write sent successfully with T_Data_SystemBroadcast service
 * not_ok: transmission of the associated T_Data_SystemBroadcast request frame did not succeed
 */

/**
 * \fn A_DomainAddress_Write.ind(priority, hop_count_type, domain_address_new)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param domain_address_new: the new value of the Domain Address
 */

/* 3.3.4 A_DomainAddress_Read-service */

/**
 * \fn A_DomainAddress_Read.req(ack_request, priority, hop_count_type)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 */

/**
 * \fn A_DomainAddress_Read.Lcon(ack_request, priority, hop_count_type, a_status)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param a_status: ok: A_DomainAddress_Read.req sent successfully with T_Data_SystemBroadcast
 * service not_ok: transmission of the associated T_Data_SystemBroadcast request frame did not
 * succeed
 */

/**
 * \fn A_DomainAddress_Read.ind(priority, hop_count_type)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 */

/**
 * \fn A_DomainAddress_Read.res(ack_request, priority, hop_count_type, domain_address)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param domain_address: the value of the Domain Address
 */

/**
 * \fn A_DomainAddress_Read.Acon(priority, hop_count_type, domain_address)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param domain_address: the value of the Domain Address
 */

/* 3.3.5 A_DomainAddressSelective_Read-service */

/**
 * \fn A_DomainAddressSelective_Read.req(priority, hop_count_type, ASDU)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param ASDU: service specific parameters
 */

/**
 * \fn A_DomainAddressSelective_Read.Lcon(priority, hop_count_type, domain_address, start_address,
 * range, a_status) \param priority: system, urgent, normal or low priority \param hop_count_type:
 * hop count 7 or standard \param ASDU: service specific parameters \param a_status: ok:
 * A_DomainAddressSelective_Read-PDU sent successfully with T_Data_SystemBroadcast service not_ok:
 * transmission of the associated T_Data_SystemBroadcast request frame did not succeed.
 */

/**
 * \fn A_DomainAddressSelective_Read.ind(priority, hop_count_type, domain_address, start_address,
 * range) \param priority: system, urgent, normal or low priority \param hop_count_type: hop count 7
 * or standard \param ASDU: service specific parameters
 */

/* 3.3.6 A_DomainAddressSerialNumber_Read-service */

/**
 * \fn A_DomainAddressSerialNumber_Read.req (ack_request, hop_count_type, priority, serial_number)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system
 * \param serial_number: the KNX Serial Number of the remote device
 */

/**
 * \fn A_DomainAddressSerialNumber_Read.Lcon (ack_request, hop_count_type, priority, serial_number,
 * a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * hop_count_type: hop count 7 or standard \param priority: system \param serial_number: the KNX
 * Serial Number of the remote device \param a_status ok:  A_DomainAddressSerialNumber_Read-PDU sent
 * successfully with T_Data_Broadcast service not_ok: transmission of the associated
 * T_Data_Broadcast request frame did not succeed
 */

/**
 * \fn A_DomainAddressSerialNumber_Read.ind (hop_count_type, priority, serial_number)
 * \param hop_count_type: hop count 7 or standard
 * \param priority: system
 * \param serial_number: the KNX Serial Number of the remote device
 */

/**
 * \fn A_DomainAddressSerialNumber_Read.res (ack_request, domain_address, hop_count_type, priority,
 * serial_number) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * domain_address: the Domain Address of the remote device \param hop_count_type: hop count 7 or
 * standard \param priority: system \param serial_number: the KNX Serial Number of the remote device
 */

/**
 * \fn A_DomainAddressSerialNumber_Read.con (domain_address, hop_count_type, priority,
 * serial_number) \param domain_address: the Domain Address of the remote device \param
 * hop_count_type: hop count 7 or standard \param priority: system \param serial_number: the KNX
 * Serial Number
 */

/* 3.3.7  A_DomainAddressSerialNumber_Write-Service */

/**
 * \fn A_DomainAddressSerialNumber_Write.req(ack_request, domain_address, hop_count_type, priority,
 * serial_number) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * domain_address: the new value of the Domain Address \param hop_count_type: hop count 7 or
 * standard \param priority: system \param serial_number: the KNX Serial Number of the remote device
 */

/**
 * \fn A_DomainAddressSerialNumber_Write.ind(domain_address, hop_count_type, priority,
 * serial_number) \param domain_address: the new value of the Domain Address \param hop_count_type:
 * hop count 7 or standard \param priority: system, urgent, normal or low priority \param
 * serial_number: the KNX Serial Number of the remote device
 */

/* 3.3.8 A_SystemNetworkParameter_Read */

/**
 * \fn A_SystemNetworkParameter_Read.req(hop_count_type, parameter_type, priority, test_info)
 * \param hop_count_type: hop count 7 or standard
 * \param parameter_type: network parameter type that is verified, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * test_info: value against which the resource indicated by parameter_type is tested
 */

/**
 * \fn A_SystemNetworkParameter_Read.Lcon(hop_count_type, parameter_type, priority, test_info,
 * a_status) \param hop_count_type: hop count 7 or standard \param parameter_type: network parameter
 * type that is verified, structured as Interface Object Type and Property Identifier \param
 * priority: system, urgent, normal or low priority \param test_info value against which the
 * resource indicated by parameter_type is tested \param a_status: ok: A_NetworkParameter_Read.req
 * sent successfully with T_Data_Broadcast – or with T_Data_SystemBroadcast-service not_ok:
 * transmission of the associated T_Data_Broadcast – or T_DataSystem_Broadcast request frame did not
 * succeed
 */

/**
 * \fn A_SystemNetworkParameter_Read.ind(hop_count_type, parameter_type, priority, test_info)
 * \param hop_count_type: hop count 7 or standard
 * \param parameter_type: network parameter type that is verified, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * test_info: value against which the resource indicated by parameter_type is tested
 */

/**
 * \fn A_SystemNetworkParameter_Read.res(hop_count_type, parameter_type, priority, test_info,
 * test_result) \param hop_count_type: hop count 7 or standard \param parameter_type: network
 * parameter type that is verified, structured as Interface Object Type and Property Identifier
 * \param priority: system, urgent, normal or low priority
 * \param test_info: value against which the resource indicated by parameter_type is tested
 * \param test_result: parameter_type dependent response
 */

/**
 * \fn A_SystemNetworkParameter_Read.Rcon(hop_count_type, parameter_type, priority, test_info,
 * test_result, a_status) \param hop_count_type: hop count 7 or standard \param parameter_type:
 * network parameter type that is verified, structured as Interface Object Type and Property
 * Identifier \param priority: system, urgent, normal or low priority \param test_info: value
 * against which the resource indicated by parameter_type is tested \param test_result:
 * parameter_type dependent response \param a_status: ok: A_NetworkParameter_Read.res sent
 * successfully with T_Data_Broadcast or T_Data_Individual service not_ok:  transmission of the
 * associated T_Data_Broadcast or T_Data_Individual request frame did not succeed
 */

/**
 * \fn A_SystemNetworkParameter_Read.Acon(ASAP, hop_count_type, parameter_type, priority, test_info,
 * test_result) \param ASAP: local reference of the Service Access Point or individual address
 * \param hop_count_type: hop count 7 or standard
 * \param parameter_type: network parameter type that is verified, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * test_info: value against which the resource indicated by parameter_type is tested \param
 * test_result: parameter_type dependent response
 */

/* 3.3.9 A_SystemNetworkParameter_Write */

/**
 * \fn A_SystemNetworkParameter_Write.req(hop_count_type, parameter_type, priority, value)
 * \param hop_count_type: hop count 7 or standard
 * \param parameter_type: the network parameter that shall be set, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * value: value to which the network parameter indicated by parameter_type shall be set
 */

/**
 * \fn A_SystemNetworkParameter_Write.Lcon(hop_count_type, parameter_type, priority, value,
 * a_status) \param hop_count_type: hop count 7 or standard \param parameter_type: the network
 * parameter that shall be set, structured as Interface Object Type and Property Identifier \param
 * priority: system, urgent, normal or low priority \param value: value to which the network
 * parameter indicated by parameter_type shall be set \param a_status: ok:
 * A_SystemNetworkParameter_Write.req sent successfully with T_Data_Broadcast – or with
 * T_Data_SystemBroadcast- service not_ok: Transmission of the associated T_Data_Broadcast – or with
 * T_Data_System- Broadcast request frame did not succeed
 */

/**
 * \fn A_SystemNetworkParameter_Write.ind(hop_count_type, parameter_type, priority, value)
 * \param hop_count_type: hop count 7 or standard
 * \param parameter_type: the network parameter that shall be set, structured as Interface Object
 * Type and Property Identifier \param priority: system, urgent, normal or low priority \param
 * value: value to which the network parameter indicated by parameter_type shall be set
 */

/* 3.4 Application Layer Services on Point-to-point Connectionless Communication Mode */

/* 3.4.2 Common services */

/* 3.4.2.1 A_DeviceDescriptor_Read-service */

/**
 * \fn A_DeviceDescriptor_Read.req(ack_request, priority, hop_count_type, ASAP, descriptor_type)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param ASAP: local reference of the Service Access Point
 * \param descriptor_type type of the Device Descriptor
 */

/**
 * \fn A_DeviceDescriptor_Read.Lcon(ack_request, priority, hop_count_type, ASAP, descriptor_type,
 * a_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * priority: system, urgent, normal or low priority \param hop_count_type: hop count 7 or standard
 * \param ASAP: local reference of the Service Access Point
 * \param descriptor_type type of the Device Descriptor
 * \param a_status: ok: A_DeviceDescriptor_Read-PDU sent successfully with T_Data_Individual service
 * not_ok: transmission of the associated T_Data_Connected request frame did not succeed
 */

/**
 * \fn A_DeviceDescriptor_Read.ind(priority, hop_count_type, ASAP, descriptor_type)
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type: hop count 7 or standard
 * \param ASAP: local reference of the Service Access Point
 * \param descriptor_type type of the Device Descriptor
 */

/**
 * \fn A_DeviceDescriptor_Read.res(ack_request, priority, hop_count_type, ASAP, descriptor_type,
 * device_descriptor) \param ck_request: Data Link Layer Acknowledge requested or don’t care \param
 * priority: system, urgent, normal or low priority \param hop_count_type: hop count 7 or standard
 * \param ASAP: local reference of the Service Access Point
 * \param descriptor_type type of the Device Descriptor
 * \param device_descriptor: the Device descriptor of the communication controller
 */

/**
 * \fn A_DeviceDescriptor_Read.Acon(priority, hop_count_type, ASAP, descriptor_type,
 * device_descriptor) \param priority: system, urgent, normal or low priority \param hop_count_type:
 * hop count 7 or standard \param ASAP: local reference of the Service Access Point \param
 * descriptor_type type of the Device Descriptor \param device_descriptor: the Device Descriptor of
 * the communication controller
 */

/* 3.4.2.2 A_Restart-service */

/**
 * \fn A_Restart.req(ack_request, channel_number, erase_code, priority, hop_count_type,
 * restart_type, ASAP) \param ack_request Data Link Layer acknowledge requested or don’t care \param
 * channel_number number of the application channel that shall be reset or 00h \param erase_code
 * indication of Resources that shall be reset prior to resetting the device \param priority:
 * system, urgent, normal or low priority \param hop_count_type hop count 7 or standard \param
 * restart_type Basic Restart or Master Reset \param ASAP: local reference of the service access
 * point or Individual Address
 */

/**
 * \fn A_Restart.ind(erase_code, channel_number, priority, hop_count_type, ASAP)
 * \param channel_number number of the application channel that shall be reset or 00h
 * \param erase_code indication of Resources that shall be reset prior to resetting the device
 * \param priority: system, urgent, normal or low priority
 * \param hop_count_type hop count 7 or standard
 * \param restart_type Basic Restart or Master Reset
 * \param ASAP: local reference of the service access point or Individual Address
 */

/**
 * \fn A_Restart.res(error_code, priority, process time, hop_count_type, ASAP)
 * \param error_code indication about success or failure of the Master Reset request
 * \param priority: system, urgent, normal or low priority
 * \param process time: worst case time required by the remote Application Layer user needs for the
 * execution of the requested Master Reset \param hop_count_type hop count 7 or standard \param
 * restart_type Basic Restart or Master Reset \param ASAP: local reference of the service access
 * point or Individual Address
 */

/* 3.4.2.3 A_FileStream_InfoReport */

/**
 * \fn A_FileStream_InfoReport.req(ack_request, ASAP, file_block, file_block_sequence_number,
 * file_handle, hop_count_type) \param ack_request: Data Link Layer Acknowledge requested or don’t
 * care \param ASAP: local reference of the Service Access Point or Individual Address \param
 * file_block: part of the file that shall be transferred to the communication partner \param
 * file_block_sequence_number:rolling counter to denote the relative position of the file block in
 * the stream transfer \param file_handle: reference to the file path of file stream to which the
 * file block belongs \param hop_count_type: hop count 7 or standard
 */

/**
 * \fn A_FileStream_InfoReport.Lcon(ack_request, ASAP, file_block, file_block_sequence_number,
 * file_handle, hop_count_type, a_status) \param ack_request: Data Link Layer Acknowledge requested
 * or don’t care \param file_block: part of the file that shall be transferred to the communication
 * partner \param file_block_sequence_number:rolling counter to denote the relative position of the
 * file block in the stream transfer \param file_handle: reference to the file path of file stream
 * to which the file block belongs \param ASAP: local reference of the Service Access Point or
 * Individual Address \param hop_count_type: hop count 7 or standard \param a_status: ok:
 * A_FileStream_InfoReport–PDU is sent successfully with T_Data_Individual service not_ok:
 * transmission of the associated T_Data_Individual request frame did not succeed
 */

/**
 * \fn A_FileStream_InfoReport.ind(ASAP, file_block, file_block_sequence_number, file_handle,
 * hop_count_type) \param ASAP: local reference of the Service Access Point or Individual Address
 * \param file_block: part of the file that shall be transferred to the communication partner
 * \param file_block_sequence_number: rolling counter to denote the relative position of the file
 * block in the stream transfer \param file_handle: reference to the file path of file stream to
 * which the file block belongs \param hop_count_type: hop count 7 or standard
 */

/* 3.4.3  Data Property services */

/* 3.4.3.1 A_PropertyValue_Read-service */



/* 3.5.1 */

void A_Connect__ind(KnxAddress source);




#endif
