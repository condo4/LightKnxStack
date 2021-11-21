#include <string.h>
#include "object_device.h"

#define USER_STOPPED 0x1
#define OWN_ADDR_DUPL 0x2
#define VERIFY_MODE 0x4
#define SAFE_STATE 0x8

//static uint16_t _ownAddress = 0x1152; /* 1.1.82 */
static uint16_t _ownAddress = 0; /* TODO: RESTORE */
static volatile uint8_t _deviceControl = 0;
static volatile uint8_t _routingCount = 0;
static volatile uint8_t _prgMode = 0;
static uint16_t _manufacturerId = 0xfa; // Default to KNXA
static uint32_t _bauNumber = 0;
static char _orderNumber[10] = "";
static uint8_t _hardwareType[6] = { 0, 0, 0, 0, 0, 0 };
static uint16_t _version = 0;
static uint16_t _maskVersion = 0x0000;
static uint16_t _maxApduLength = 254;
static const uint32_t *_ifObjs;

static struct property_description _property_descriptions[] = {
    { PID_OBJECT_TYPE, false, PDT_UNSIGNED_INT, 1, ReadLv3 | WriteLv0 },
    { PID_SERIAL_NUMBER, false, PDT_GENERIC_06, 1, ReadLv3 | WriteLv0 }
};

void device_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                      bool *writeEnable, uint8_t *type, uint16_t *numberOfElements,
                                      uint8_t *access)
{
    interface_read_property_description(
            _property_descriptions,
            sizeof(_property_descriptions) / sizeof(struct property_description), propertyId,
            propertyIndex, writeEnable, type, numberOfElements, access);
}

void device_read_property(PropertyID propertyId, uint32_t start, uint32_t *count, uint8_t *data)
{
    int i;
    switch (propertyId)
    {
    case PID_OBJECT_TYPE:
        data[0] = ((OT_DEVICE >> 8) & 0xff);
        data[1] = (OT_DEVICE & 0xff);
        break;
    case PID_SERIAL_NUMBER:
        data[0] = ((_manufacturerId >> 8) & 0xff);
        data[1] = (_manufacturerId & 0xff);
        data[2] = ((_bauNumber >> 24) & 0xff);
        data[3] = ((_bauNumber >> 16) & 0xff);
        data[4] = ((_bauNumber >> 8) & 0xff);
        data[5] = (_bauNumber & 0xff);
        break;
    case PID_MANUFACTURER_ID:
        data[0] = ((_manufacturerId >> 8) & 0xff);
        data[1] = (_manufacturerId & 0xff);
        break;
    case PID_DEVICE_CONTROL:
        data[0] = _deviceControl;
        break;
    case PID_ORDER_INFO:
        for (i = 0; i < 10; i++)
            data[i] = ((uint8_t *)_orderNumber)[i];
        break;
    case PID_HARDWARE_TYPE:
        for (i = 0; i < 6; i++)
            data[i] = ((uint8_t *)_hardwareType)[i];
        break;
    case PID_VERSION:
        data[0] = ((_version >> 8) & 0xff);
        data[1] = (_version & 0xff);
        break;
    case PID_ROUTING_COUNT:
        data[0] = _routingCount;
        break;
    case PID_PROG_MODE:
        data[0] = _prgMode;
        break;
    case PID_MAX_APDU_LENGTH:
        data[0] = ((_maxApduLength >> 8) & 0xff);
        data[1] = (_maxApduLength & 0xff);
        break;
    case PID_SUBNET_ADDR:
        data[0] = ((_ownAddress >> 8) & 0xff);
        break;
    case PID_DEVICE_ADDR:
        data[0] = (_ownAddress & 0xff);
        break;
    case PID_IO_LIST: {
        int o = 0;
        for (i = start; i < (_ifObjs[0] + 1) && i < (*count); i++)
        {
            data[o + 0] = ((_ifObjs[i] >> 24) & 0xff);
            data[o + 1] = ((_ifObjs[i] >> 16) & 0xff);
            data[o + 2] = ((_ifObjs[i] >> 8) & 0xff);
            data[o + 3] = (_ifObjs[i] & 0xff);
            o += 4;
        }
        break;
    }
    case PID_DEVICE_DESCRIPTOR:
        data[0] = ((_maskVersion >> 8) & 0xff);
        data[1] = (_maskVersion & 0xff);
        break;
    default:
        (*count) = 0;
    }
}

void device_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count)
{
    switch (id)
    {
    case PID_DEVICE_CONTROL:
        _deviceControl = data[0];
        break;
    case PID_ROUTING_COUNT:
        _routingCount = data[0];
        break;
    case PID_PROG_MODE:
        _prgMode = data[0];
        break;
    }
}

uint16_t device_induvidual_address()
{
    return _ownAddress;
}

void device_induvidual_address_set(uint16_t value)
{
    _ownAddress = value;
}

bool device_induvidual_address_duplication()
{
    return (_deviceControl & OWN_ADDR_DUPL) > 0;
}

void device_induvidual_address_duplication_set(bool value)
{
    if (value)
        _deviceControl |= OWN_ADDR_DUPL;
    else
        _deviceControl &= ~OWN_ADDR_DUPL;
}

bool device_prog_mode()
{
    return _prgMode == 1;
}

void device_prog_mode_set(bool value)
{
    if (value)
    {
        _prgMode = 1;
    }
    else
    {
        _prgMode = 0;
    }
    device_prog_mode_change(_prgMode);
}

uint16_t device_manufacturer_id()
{
    return _manufacturerId;
}

void device_manufacturer_id_set(uint16_t value)
{
    _manufacturerId = value;
}

uint32_t device_bau_number()
{
    return _bauNumber;
}

void device_bau_number_set(uint32_t value)
{
    _bauNumber = value;
}

const uint32_t *device_if_obj()
{
    return _ifObjs;
}

void device_if_obj_set(const uint32_t *value)
{
    _ifObjs = value;
}

uint16_t device__mask_version()
{
    return _maskVersion;
}

void device_mask_version_set(uint16_t value)
{
    _maskVersion = value;
}

bool device_verify_mode()
{
    return (_deviceControl & VERIFY_MODE) > 0;
}

void device_verify_mode_set(bool value)
{
    if (value)
        _deviceControl |= VERIFY_MODE;
    else
        _deviceControl &= ~VERIFY_MODE;
}

bool device_user_stopped()
{
    return (_deviceControl & USER_STOPPED) > 0;
}

void device_user_stopped_set(bool value)
{
    if (value)
        _deviceControl |= USER_STOPPED;
    else
        _deviceControl &= ~USER_STOPPED;
}

bool device_safe_state()
{
    return (_deviceControl & SAFE_STATE) > 0;
}

void device_safe_state_set(bool value)
{
    if (value)
        _deviceControl |= SAFE_STATE;
    else
        _deviceControl &= ~SAFE_STATE;
}

const char *device_order_number()
{
    return _orderNumber;
}

void device_order_number_set(const char *value)
{
    strncpy(_orderNumber, value, 10);
}

const uint8_t *device_hardware_type()
{
    return _hardwareType;
}

void device_hardware_type_set(const uint8_t *value)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        _hardwareType[i] = ((uint8_t *)value)[i];
    }
}

uint16_t device_version()
{
    return _version;
}

void device_version_set(uint16_t value)
{
    _version = value;
}

uint16_t device_max_apdu_length()
{
    return _maxApduLength;
}

void device_max_apdu_length_set(uint16_t value)
{
    _maxApduLength = value;
}

uint8_t *device_object_save(uint8_t *buffer)
{
    buffer[0] = _deviceControl;
    buffer[1] = _routingCount;
    buffer[2] = ((_ownAddress >> 8) & 0xff);
    buffer[3] = (_ownAddress & 0xff);
    return buffer + 4;
}

uint8_t *device_object_restore(uint8_t *buffer)
{
    _deviceControl = buffer[0];
    _routingCount = buffer[1];
    _ownAddress = (buffer[2] << 8) + buffer[3];
    _prgMode = 0;
    return buffer + 4;
}

uint8_t device_object_property_size(PropertyID id)
{
    switch (id)
    {
    case PID_DEVICE_CONTROL:
    case PID_ROUTING_COUNT:
    case PID_PROG_MODE:
    case PID_SUBNET_ADDR:
    case PID_DEVICE_ADDR:
        return 1;
    case PID_OBJECT_TYPE:
    case PID_MANUFACTURER_ID:
    case PID_VERSION:
    case PID_DEVICE_DESCRIPTOR:
    case PID_MAX_APDU_LENGTH:
        return 2;
    case PID_IO_LIST:
        return 4;
    case PID_SERIAL_NUMBER:
    case PID_HARDWARE_TYPE:
        return 6;
    case PID_ORDER_INFO:
        return 10;
    }
    return 0;
}
