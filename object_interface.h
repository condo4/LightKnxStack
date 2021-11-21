#ifndef __OBJECT_INTERFACE__
#define __OBJECT_INTERFACE__

#include <stddef.h>
#include "object_property_types.h"

#define htons(x) ((((x) << 8) & 0xFF00) | (((x) >> 8) & 0xFF))
#define ntohs(x) htons(x)
#define htonl(x)                                                                                   \
    (((x) << 24 & 0xFF000000UL) | ((x) << 8 & 0x00FF0000UL) | ((x) >> 8 & 0x0000FF00UL)            \
     | ((x) >> 24 & 0x000000FFUL))
#define ntohl(x) htonl(x)

#ifdef __cplusplus
extern "C" {
#endif

void interface_read_property_description(struct property_description *descriptions, uint8_t count,
                                         uint8_t *propertyId, uint8_t *propertyIndex,
                                         bool *writeEnable, uint8_t *type,
                                         uint16_t *numberOfElements, uint8_t *access);

/** Enum for the type of an interface object. See Section 2.2 of knx:3/7/3 */
enum ObjectType
{
    /** Device object. */
    OT_DEVICE = 0,

    /** Address table object. */
    OT_ADDR_TABLE = 1,

    /** Association table object. */
    OT_ASSOC_TABLE = 2,

    /** Application program object. */
    OT_APPLICATION_PROG = 3,

    /** Interface program object. */
    OT_INTERFACE_PROG = 4,

    /** KNX - Object Associationtable. */
    OT_OJB_ASSOC_TABLE = 5,

    /** Router Object */
    OT_ROUTER = 6,

    /** LTE Address Routing Table Object */
    OT_LTE_ADDR_ROUTING_TABLE = 7,

    /** cEMI Server Object */
    OT_CEMI_SERVER = 8,

    /** Group Object Table Object */
    OT_GRP_OBJ_TABLE = 9,

    /** Polling Master */
    OT_POLLING_MASTER = 10,

    /** KNXnet/IP Parameter Object */
    OT_IP_PARAMETER = 11,

    /** Reserved. Shall not be used. */
    OT_RESERVED = 12,

    /** File Server Object */
    OT_FILE_SERVER = 13,

    /** RF Medium Object */
    OT_RF_MEDIUM = 19
};

#ifdef __cplusplus
}
#endif

#endif
