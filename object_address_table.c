#include <string.h>

#include "object_address_table.h"
#include "object_table.h"
#include "object_interface.h"

static struct table_object _table;
static volatile uint16_t *_groupAddresses = 0;

static struct property_description _property_descriptions[] = {
    { PID_OBJECT_TYPE, false, PDT_UNSIGNED_INT, 1, ReadLv3 | WriteLv0 },
    { PID_LOAD_STATE_CONTROL, true, PDT_CONTROL, 1, ReadLv3 | WriteLv3 },
    { PID_TABLE_REFERENCE, false, PDT_UNSIGNED_LONG, 1, ReadLv3 | WriteLv0 },
    { PID_ERROR_CODE, false, PDT_ENUM8, 1, ReadLv3 | WriteLv0 },
};

LoadState address_table_load_state()
{
    return _table.state;
}

void address_table_read_property(PropertyID id, uint32_t start, uint32_t *count, uint8_t *data)
{
    switch (id)
    {
    case PID_OBJECT_TYPE:
        _table.data[0] = ((OT_ADDR_TABLE >> 8) & 0xff);
        _table.data[1] = (OT_ADDR_TABLE & 0xff);
        break;
    default:
        table_read_property(&_table, id, start, count, data);
    }
}

static uint16_t entry_count()
{
    if (_table.state != LS_LOADED)
        return 0;

    return ntohs(_groupAddresses[0]);
}

void address_table_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                             bool *writeEnable, uint8_t *type,
                                             uint16_t *numberOfElements, uint8_t *access)
{
    interface_read_property_description(
            _property_descriptions,
            sizeof(_property_descriptions) / sizeof(struct property_description), propertyId,
            propertyIndex, writeEnable, type, numberOfElements, access);
}

void address_table_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count)
{
    table_write_property(&_table, id, start, data, count);
}

uint8_t *address_table_save(uint8_t *buffer)
{
    return table_object_save(&_table, buffer);
}

uint8_t *address_table_restore(uint8_t *buffer)
{
    buffer = table_object_restore(&_table, buffer);
    _groupAddresses = (uint16_t *)(_table.data);
    return buffer;
}

static void address_table_before_state_change(struct table_object *table, LoadState *newState)
{
    if ((*newState) != LS_LOADED)
        return;

    _groupAddresses = (uint16_t *)_table.data;
}

uint8_t address_table_property_count()
{
    return sizeof(_property_descriptions) / sizeof(struct property_description);
}

struct property_description *address_table_property_descriptions()
{
    return _property_descriptions;
}

void init_address_table_object()
{
    init_table_object(&_table);
    _table.before_state_change = address_table_before_state_change;
}

uint16_t address_table_get_group_address(uint16_t tsap)
{
    if (_table.state != LS_LOADED || tsap > entry_count())
        return 0;

    return ntohs(_groupAddresses[tsap]);
}

uint16_t address_table_get_tsap(uint16_t addr)
{
    uint16_t size = entry_count();
    for (uint16_t i = 1; i <= size; i++)
        if (ntohs(_groupAddresses[i]) == addr)
            return i;
    return 0xFFFF;
}

bool address_table_contains(uint16_t addr)
{
    uint16_t size = entry_count();
    for (uint16_t i = 1; i <= size; i++)
        if (ntohs(_groupAddresses[i]) == addr)
            return true;

    return false;
}

uint8_t address_table_property_size(PropertyID id)
{
    return table_property_size(id);
}
