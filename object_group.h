#ifndef __OBJECT_GROUP__
#define __OBJECT_GROUP__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "dpt.h"
#include "knx_types.h"

#if __cplusplus
extern "C" {
#endif

enum ComFlag_
{
    Updated = 0, /* Group object was updated */
    ReadRequest = 1, /* Read was requested but was not processed */
    WriteRequest = 2, /* Write was requested but was not processed */
    Transmitting = 3, /* Group Object is processed a the moment (read or write) */
    Ok = 4, /* read or write request were send successfully */
    Error = 5 /* there was an error on processing a request */
};
typedef uint8_t ComFlag;

struct group_object;

typedef void (*GroupObjectUpdatedHandler)(struct group_object *go);

struct group_object
{
    GroupObjectUpdatedHandler updateHandler;
    uint16_t asap;
    ComFlag commFlag;
    uint8_t *data;
    uint8_t dataLength;
    struct dpt datapointType;
};

#define MASK_FLAG_COMMUNICATION 0x400
#define MASK_FLAG_READ 0x800

#define COMMUNICATION_ENABLED(go)                                                                  \
    ((ntohs(group_object_table_data(go->asap)) & MASK_FLAG_COMMUNICATION) != 0)
#define READ_ENABLED(go) ((ntohs(group_object_table_data(go->asap)) & MASK_FLAG_READ) != 0)

#define SIZE_IN_TELEGRAM(pgo)                                                                      \
    (asap_value_size(((ntohs(group_object_table_data(pgo->asap))) & 0xFF)))
#define EMPTY_GROUP_OBJECT                                                                         \
    {                                                                                              \
        .updateHandler = NULL, .asap = 0, .commFlag = Ok, .data = 0, .dataLength = 0, DPT(0, 0)    \
    }

void group_object_set_bool(struct group_object *obj, bool value);
void group_object_set_uint8(struct group_object *obj, uint8_t value);
void group_object_set_uint16(struct group_object *obj, uint16_t value);
void group_object_set_uint32(struct group_object *obj, uint32_t value);
void group_object_set_uint64(struct group_object *obj, uint64_t value);
void group_object_set_int8(struct group_object *obj, int8_t value);
void group_object_set_int16(struct group_object *obj, int16_t value);
void group_object_set_int32(struct group_object *obj, int32_t value);
void group_object_set_int64(struct group_object *obj, int64_t value);

void group_object_set_float16(struct group_object *obj, float value);
float group_object_get_float16(const struct group_object *obj);

void group_object_set_float32(struct group_object *obj, float value);
float group_object_get_float32(const struct group_object *obj);

void group_object_set_double(struct group_object *obj, double value);

/**
 * Check if the update flag (U) was set. (A-flag in german)
 */
bool group_object_get_response_update_enabled(const struct group_object *obj);

/**
 * Check if the transmit flag (T) was set. (UE-flag in german)
 */
bool group_object_get_transmit_enabled(const struct group_object *obj);

/**
 * Check if the initialisation flag (I) was set.
 */
bool group_object_get_value_read_on_init(const struct group_object *obj);

/**
 * Check if the write flag (W) was set. (S-flag in german)
 */
bool group_object_get_write_enabled(const struct group_object *obj);

/**
 * Check if the read flag (R) was set. (L-flag in german)
 */
bool group_object_get_read_enabled(const struct group_object *obj);

/**
 * Check if the communication flag (C) was set. (K-flag in german)
 */
bool group_object_get_communication_enabled(const struct group_object *obj);

/**
 * Get the priority of the group object.
 */
Priority group_object_get_priority(const struct group_object *obj);

/**
 * Return the current state of the group object. See ::ComFlag
 */
ComFlag group_object_get_comm_flag(const struct group_object *obj);

/**
 * Set the current state of the group object. Application code should only use this to set the state
 * to ::Ok after reading a ::Updated to mark the changed group object as processed. This is
 * optional.
 */
void group_object_set_comm_flag(struct group_object *obj, ComFlag value);

/**
 * Request the read of a communication object. Calling this function triggers the
 * sending of a read-group-value telegram, to read the value of the communication
 * object from the bus.
 *
 * When the answer is received, the communication object's value will be updated.
 *
 * This sets the state of the group objecte to ::ReadRequest
 */
void group_object_set_request_object_read(struct group_object *obj);

/**
 * Mark a communication object as written. Calling this
 * function triggers the sending of a write-group-value telegram.
 *
 * This sets the state of the group object to ::WriteRequest
 */
void group_object_set_object_written(struct group_object *obj);

/**
 * returns the size of the group object in Byte. For Group objects with size smaller than 1 byte
 * (for example Dpt 1) this method will return 1.
 */
size_t group_object_get_value_size(const struct group_object *obj);

/**
 * returns the pointer to the value of the group object. This can be used if a datapoint type is not
 * supported or if you want do your own conversion.
 */
uint8_t *group_object_get_value_ref(struct group_object *obj);

/**
 * returns the Application Service Access Point of the group object. In reality this is just the
 * number of the group object. (in german "KO-Nr")
 */
uint16_t group_object_get_asap(const struct group_object *obj);

/**
 * returns the registered callback
 */
GroupObjectUpdatedHandler group_object_get_callback(const struct group_object *obj);

/**
 * register a callback for this group object. The registered callback will be called if the group
 * object was changed from the bus.
 */
void group_object_set_callback(struct group_object *obj, GroupObjectUpdatedHandler handler);

/**
 * returns the currently configured datapoint type.
 */
struct dpt group_object_get_data_point_type(const struct group_object *obj);

/**
 * sets the datapoint type of the group object.
 */
void group_object_set_data_point_type(struct group_object *obj, const struct dpt value);

size_t group_object_get_size(const struct group_object *obj);

// see knxspec 3.5.1 p. 178
static inline size_t asap_value_size(uint8_t code)
{
    if (code < 7)
        return 0;
    if (code < 8)
        return 1;
    if (code < 11 || (code > 20 && code < 255))
        return code - 6;
    switch (code)
    {
    case 11:
        return 6;
    case 12:
        return 8;
    case 13:
        return 10;
    case 14:
        return 14;
    case 15:
        return 5;
    case 16:
        return 7;
    case 17:
        return 9;
    case 18:
        return 11;
    case 19:
        return 12;
    case 20:
        return 13;
    case 255:
        return 252;
    }
    return -1;
}

#if __cplusplus
}
#endif

#endif
