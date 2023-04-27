#include <stdio.h>
#include "object_application_program.h"
#include "object_interface.h"

static struct table_object _table;
static uint8_t _programVersion[5] = { 0, 0, 0, 0, 0 };

static struct property_description _property_descriptions[] = {
    { PID_OBJECT_TYPE, false, PDT_UNSIGNED_INT, 1, ReadLv3 | WriteLv0 },
    { PID_LOAD_STATE_CONTROL, true, PDT_CONTROL, 1, ReadLv3 | WriteLv3 },
    { PID_TABLE_REFERENCE, false, PDT_UNSIGNED_LONG, 1, ReadLv3 | WriteLv0 },
    { PID_ERROR_CODE, false, PDT_ENUM8, 1, ReadLv3 | WriteLv0 },
    { PID_PEI_TYPE, false, PDT_UNSIGNED_CHAR, 1, ReadLv3 | WriteLv0 },
    { PID_PROG_VERSION, true, PDT_GENERIC_05, 1, ReadLv3 | WriteLv3 },
};

LoadState application_program_object_load_state()
{
    return _table.state;
}

void application_program_object_read_property_description(uint8_t *propertyId,
                                                          uint8_t *propertyIndex, bool *writeEnable,
                                                          uint8_t *type, uint16_t *numberOfElements,
                                                          uint8_t *access)
{
    interface_read_property_description(
            _property_descriptions,
            sizeof(_property_descriptions) / sizeof(struct property_description), propertyId,
            propertyIndex, writeEnable, type, numberOfElements, access);
}

void application_program_object_write_property(PropertyID id, uint8_t start, uint8_t *data,
                                               uint8_t count)
{
    switch (id)
    {
    case PID_PROG_VERSION:
        for (uint32_t i = 0; i < 5; i++)
        {
            _programVersion[i] = data[i];
        }
        break;
    default:
        table_write_property(&_table, id, start, data, count);
    }
}

void application_program_object_read_property(PropertyID id, uint32_t start, uint32_t *count,
                                              uint8_t *data)
{
    int i;

    switch (id)
    {
    case PID_OBJECT_TYPE:
        data[0] = ((OT_APPLICATION_PROG >> 8) & 0xff);
        data[1] = (OT_APPLICATION_PROG & 0xff);
        break;
    case PID_PROG_VERSION:
        for (i = 0; i < 5; i++)
            data[i] = _programVersion[i];
        break;
    case PID_PEI_TYPE:
        data[0] = 0;
        break;
    default:
        table_read_property(&_table, id, start, count, data);
    }
}

uint8_t application_program_object_property_size(PropertyID id)
{
    switch (id)
    {
    case PID_PEI_TYPE:
        return 1;
    case PID_OBJECT_TYPE:
        return 2;
    case PID_PROG_VERSION:
        return 5;
    case PID_LOAD_STATE_CONTROL:
        /* TODO: need to check */
        break;
    }
    return table_property_size(id);
}

uint8_t *application_program_object_data(uint32_t addr)
{
    return _table.data + addr;
}

uint8_t application_program_object_get_byte(uint32_t addr)
{
    return *(_table.data + addr);
}

uint16_t application_program_object_get_word(uint32_t addr)
{

    return ((_table.data + addr)[0] << 8) + (_table.data + addr)[1];
}

uint32_t application_program_object_get_int(uint32_t addr)
{
    return ((_table.data + addr)[0] << 24) + ((_table.data + addr)[1] << 16)
            + ((_table.data + addr)[2] << 8) + (_table.data + addr)[3];
}

uint8_t *application_program_object_save(uint8_t *buffer)
{
    for (uint32_t i = 0; i < 5; i++)
        buffer[i] = _programVersion[i];
    buffer += 5;
    return table_object_save(&_table, buffer);
}

uint8_t *application_program_object_restore(uint8_t *buffer)
{
    for (uint32_t i = 0; i < 5; i++)
        _programVersion[i] = buffer[i];
    buffer += 5;
    buffer = table_object_restore(&_table, buffer);

    return buffer;
}

void init_application_program()
{
    init_table_object(&_table);
}
