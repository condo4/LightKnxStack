#include <string.h>
#include <stdlib.h>

#include "object_group_table.h"
#include "object_group.h"
#include "object_interface.h"

static struct table_object _table;
static uint16_t *_tableData = 0;
static struct group_object *_groupObjects = 0;
static uint16_t _groupObjectCount = 0;

static struct property_description _property_descriptions[] = {
    { PID_OBJECT_TYPE, false, PDT_UNSIGNED_INT, 1, ReadLv3 | WriteLv0 },
    { PID_LOAD_STATE_CONTROL, true, PDT_CONTROL, 1, ReadLv3 | WriteLv3 },
    { PID_TABLE_REFERENCE, false, PDT_UNSIGNED_LONG, 1, ReadLv3 | WriteLv0 },
    { PID_ERROR_CODE, false, PDT_ENUM8, 1, ReadLv3 | WriteLv0 },
};

LoadState group_object_table_load_state()
{
    return _table.state;
}

void group_object_table_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                                  bool *writeEnable, uint8_t *type,
                                                  uint16_t *numberOfElements, uint8_t *access)
{
    interface_read_property_description(
            _property_descriptions,
            sizeof(_property_descriptions) / sizeof(struct property_description), propertyId,
            propertyIndex, writeEnable, type, numberOfElements, access);
}

void group_object_table_read_property(PropertyID id, uint32_t start, uint32_t *count, uint8_t *data)
{
    table_read_property(&_table, id, start, count, data);
}

void group_object_table_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count)
{
    table_write_property(&_table, id, start, data, count);
}

uint16_t group_object_table_data(uint16_t asap)
{
    return _tableData[asap];
}

static void free_group_objects()
{
    if (_groupObjects)
    {
        free(_groupObjects);
        _groupObjects = NULL;
    }

    _groupObjectCount = 0;
    _groupObjects = 0;
}

static bool init_group_objects()
{
    if (!_tableData)
        return false;

    free_group_objects();

    uint16_t goCount = ntohs(_tableData[0]);

    _groupObjects = (struct group_object *)calloc(sizeof(struct group_object), goCount);
    _groupObjectCount = goCount;

    for (uint16_t asap = 1; asap <= goCount; asap++)
    {
        struct group_object *go = &_groupObjects[asap - 1];
        go->asap = asap;

        go->dataLength = group_object_get_size(go);
        go->data = (uint8_t *)malloc(go->dataLength);
        memset(go->data, 0, go->dataLength);

        if (group_object_get_value_read_on_init(go))
            group_object_set_request_object_read(go);
    }

    return true;
}

uint8_t *group_object_table_save(uint8_t *buffer)
{
    return table_object_save(&_table, buffer);
}

uint8_t *group_object_table_restore(uint8_t *buffer)
{
    buffer = table_object_restore(&_table, buffer);

    _tableData = (uint16_t *)_table.data;
    init_group_objects();

    return buffer;
}

void init_group_object_table()
{
    init_table_object(&_table);
}

uint8_t group_object_table_property_size(PropertyID id)
{
    return table_property_size(id);
}

uint16_t group_object_table_entry_count()
{
    if (_table.state != LS_LOADED)
        return 0;

    return ntohs(_tableData[0]);
}

struct group_object *group_object_table_get(uint16_t asap)
{
    return &(_groupObjects[asap - 1]);
}
