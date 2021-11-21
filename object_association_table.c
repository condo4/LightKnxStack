#include <string.h>
#include "object_association_table.h"
#include "object_interface.h"

static struct table_object _table;
static uint16_t *_tableData = 0;

static struct property_description _property_descriptions[] = {
    { PID_OBJECT_TYPE, false, PDT_UNSIGNED_INT, 1, ReadLv3 | WriteLv0 },
    { PID_TABLE, false, PDT_GENERIC_04, 65535, ReadLv3 | WriteLv0 },
    { PID_LOAD_STATE_CONTROL, true, PDT_CONTROL, 1, ReadLv3 | WriteLv3 },
    { PID_TABLE_REFERENCE, false, PDT_UNSIGNED_LONG, 1, ReadLv3 | WriteLv0 },
    { PID_ERROR_CODE, false, PDT_ENUM8, 1, ReadLv3 | WriteLv0 },
};

LoadState association_table_load_state()
{
    return _table.state;
}

void association_table_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                                 bool *writeEnable, uint8_t *type,
                                                 uint16_t *numberOfElements, uint8_t *access)
{
    interface_read_property_description(
            _property_descriptions,
            sizeof(_property_descriptions) / sizeof(struct property_description), propertyId,
            propertyIndex, writeEnable, type, numberOfElements, access);
}

void association_table_read_property(PropertyID id, uint32_t start, uint32_t *count, uint8_t *data)
{
    switch (id)
    {
    case PID_OBJECT_TYPE:
        data[0] = ((OT_ASSOC_TABLE >> 8) & 0xff);
        data[1] = (OT_ASSOC_TABLE & 0xff);
        break;
    default:
        table_read_property(&_table, id, start, count, data);
    }
}

void association_table_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count)
{
    table_write_property(&_table, id, start, data, count);
}

uint8_t *association_table_save(uint8_t *buffer)
{
    return table_object_save(&_table, buffer);
}

uint8_t *association_table_restore(uint8_t *buffer)
{
    buffer = table_object_restore(&_table, buffer);
    _tableData = (uint16_t *)_table.data;
    return buffer;
}

static uint16_t entry_count()
{
    return htons(_tableData[0]);
}

static uint16_t get_TSAP(uint16_t idx)
{
    if (idx < 0 || idx >= entry_count())
        return 0;

    return htons(_tableData[2 * idx + 1]);
}

static uint16_t get_ASAP(uint16_t idx)
{
    if (idx < 0 || idx >= entry_count())
        return 0;

    return htons(_tableData[2 * idx + 2]);
}

void init_association_table()
{
    init_table_object(&_table);
}

int32_t association_table_next_asap(uint16_t tsap, uint16_t *startIdx)
{
    uint16_t entries = entry_count();
    for (uint16_t i = *startIdx; i < entries; i++)
    {
        *startIdx = i + 1;

        if (get_TSAP(i) == tsap)
        {
            return get_ASAP(i);
        }
    }
    return -1;
}

// return type is int32 so that we can return uint16 and -1
int32_t association_table_translate_asap(uint16_t asap)
{
    uint16_t entries = entry_count();
    for (uint16_t i = 0; i < entries; i++)
    {
        if (get_ASAP(i) == asap)
            return get_TSAP(i);
    }
    return -1;
}

uint8_t association_table_property_size(PropertyID id)
{
    return table_property_size(id);
}
