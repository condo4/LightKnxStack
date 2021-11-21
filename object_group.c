#include "object_group.h"
#include "string.h"
#include "object_group_table.h"

// DPT 1.* - Binary
#define bitRead(val, bitno) (((val) >> (bitno)) & 1)
#define htons(x) ((((x) << 8) & 0xFF00) | (((x) >> 8) & 0xFF))
#define ntohs(x) htons(x)

void group_object_set_bool(struct group_object *obj, bool value)
{
    // KNX_Encode_Value(value, obj->_data, obj->_dataLength, obj->_datapointType);
    if (obj->datapointType.mainGroup == 1 && obj->datapointType.subGroup >= 1
        && obj->datapointType.subGroup <= 23 && obj->datapointType.subGroup != 20
        && !obj->datapointType.index)
    {
        obj->data[0] = (obj->data[0] & ~1) | (value ? (1) : 0);
    }
    obj->commFlag = WriteRequest;
}

// DPT 9.* - 16 Bit Float
void group_object_set_float16(struct group_object *obj, float value)
{
    switch (obj->datapointType.subGroup)
    {
    case 1:
        if (value < -273.0)
            return;
        break;
    case 2:
    case 3:
    case 10:
    case 11:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
        if (value < -670760.0)
            return;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 28:
        if (value < 0.0)
            return;
        break;
    case 27:
        if (value < -459.6)
            return;
        break;
    }

    float16_to_payload(obj->data, obj->dataLength, 0, value, 0xFFFF);
    obj->commFlag = WriteRequest;
}

float group_object_get_float16(const struct group_object *obj)
{
    return payload_to_float16(obj->data, 0);
}

bool group_object_get_response_update_enabled(const struct group_object *obj)
{
    return bitRead(ntohs(group_object_table_data(obj->asap)), 15) > 0;
}

bool group_object_get_transmit_enabled(const struct group_object *obj)
{
    return bitRead(ntohs(group_object_table_data(obj->asap)), 14) > 0;
}

bool group_object_get_value_read_on_init(const struct group_object *obj)
{
    return bitRead(ntohs(group_object_table_data(obj->asap)), 13) > 0;
}

bool group_object_get_write_enabled(const struct group_object *obj)
{
    return bitRead(ntohs(group_object_table_data(obj->asap)), 12) > 0;
}

bool group_object_get_read_enabled(const struct group_object *obj)
{
    return bitRead(ntohs(group_object_table_data(obj->asap)), 11) > 0;
}

bool group_object_get_communication_enabled(const struct group_object *obj)
{
    return bitRead(ntohs(group_object_table_data(obj->asap)), 10) > 0;
}

Priority group_object_get_priority(const struct group_object *obj)
{
    return (Priority)((ntohs(group_object_table_data(obj->asap)) >> 6) & (3 << 2));
}

ComFlag group_object_get_comm_flag(const struct group_object *obj)
{
    return obj->commFlag;
}

void group_object_set_comm_flag(struct group_object *obj, ComFlag value)
{
    obj->commFlag = value;
}

void group_object_set_request_object_read(struct group_object *obj)
{
    obj->commFlag = ReadRequest;
}

void group_object_set_object_written(struct group_object *obj)
{
    obj->commFlag = WriteRequest;
}

size_t group_object_get_value_size(const struct group_object *obj)
{
    return obj->dataLength;
}

uint8_t *group_object_get_value_ref(struct group_object *obj)
{
    return obj->data;
}

uint16_t group_object_get_asap(const struct group_object *obj)
{
    return obj->asap;
}

GroupObjectUpdatedHandler group_object_get_callback(const struct group_object *obj)
{
    return obj->updateHandler;
}

void group_object_set_callback(struct group_object *obj, GroupObjectUpdatedHandler handler)
{
    obj->updateHandler = handler;
}

struct dpt group_object_get_data_point_type(const struct group_object *obj)
{
    return obj->datapointType;
}

void group_object_set_data_point_type(struct group_object *obj, const struct dpt value)
{
    obj->datapointType = value;
}

size_t group_object_get_size(const struct group_object *obj)
{
    size_t size = SIZE_IN_TELEGRAM(obj);
    if (size == 0)
        return 1;

    return size;
}
