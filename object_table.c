#include <string.h>
#include <stdlib.h>
#include "object_table.h"

uint8_t *table_object_save(struct table_object *table, uint8_t *buffer)
{
    int i = 0;
    int j = 0;
    buffer[i++] = table->state;
    buffer[i++] = table->error;
    buffer[i] = ((table->size >> 24) & 0xff);
    buffer[i] = ((table->size >> 16) & 0xff);
    buffer[i] = ((table->size >> 8) & 0xff);
    buffer[i] = (table->size & 0xff);
    i += 4;

    for (j = 0; j < table->size; j++)
        buffer[i + j] = table->data[j];

    return buffer;
}

uint8_t *table_object_restore(struct table_object *table, uint8_t *buffer)
{
    uint8_t state = 0;
    uint8_t errorCode = 0;

    state = *buffer;
    buffer += 1;
    errorCode = *buffer;
    buffer += 1;

    table->state = (LoadState)state;
    table->error = (ErrorCode)errorCode;

    table->size = (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
    buffer += 4;

    if (table->data)
        free(table->data);

    if (table->size > 0)
        table->data = (uint8_t *)malloc(table->size);
    else
        table->data = 0;

    for (uint32_t i = 0; i < table->size; i++)
    {
        table->data[i] = buffer[i];
    }

    buffer += table->size;

    return buffer;
}

void init_table_object(struct table_object *table)
{
    table->state = LS_UNLOADED;
    table->data = NULL;
    table->size = 0;
    table->error = E_NO_FAULT;
}

uint32_t table_reference(struct table_object *table)
{
    return (uint32_t)(table->data - platform_memory_reference());
}

void table_read_property(struct table_object *table, PropertyID id, uint32_t start, uint32_t *count,
                         uint8_t *data)
{
    switch (id)
    {
    case PID_LOAD_STATE_CONTROL:
        data[0] = table->state;
        break;
    case PID_TABLE_REFERENCE:
        if (table->state == LS_UNLOADED)
        {
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
        }
        else
        {
            data[0] = ((table_reference(table) >> 24) & 0xff);
            data[1] = ((table_reference(table) >> 16) & 0xff);
            data[2] = ((table_reference(table) >> 8) & 0xff);
            data[3] = (table_reference(table) & 0xff);
        }
        break;
    case PID_ERROR_CODE:
        data[0] = table->error;
        break;
    default:
        (*count) = 0;
    }
}

void table_load_state(struct table_object *table, LoadState newState)
{
    if (newState == table->state)
        return;
    if (table->before_state_change)
        table->before_state_change(table, &newState);
    table->state = newState;
}

static void table_load_event_unloaded(struct table_object *table, uint8_t *data)
{
    uint8_t event = data[0];
    switch (event)
    {
    case LE_NOOP:
    case LE_LOAD_COMPLETED:
    case LE_ADDITIONAL_LOAD_CONTROLS:
    case LE_UNLOAD:
        break;
    case LE_START_LOADING:
        table_load_state(table, LS_LOADING);
        break;
    default:
        table_load_state(table, LS_ERROR);
        table->error = E_GOT_UNDEF_LOAD_CMD;
    }
}

static int table_allocate(struct table_object *table, uint32_t size, bool doFill, uint8_t fillByte)
{
    if (table->data)
    {
        free(table->data);
        table->data = 0;
        table->size = 0;
    }

    if (size == 0)
        return true;

    table->data = (uint8_t *)malloc(size);
    if (!table->data)
        return false;

    table->size = size;

    if (doFill)
        memset(table->data, fillByte, size);

    return true;
}

static void table_additional_load_controls(struct table_object *table, uint8_t *data)
{
    if (data[1] != 0x0B) // Data Relative Allocation
    {
        table_load_state(table, LS_ERROR);
        table->error = E_INVALID_OPCODE;
        return;
    }

    size_t size = ((data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5]);
    bool doFill = data[6] == 0x1;
    uint8_t fillByte = data[7];
    if (!table_allocate(table, size, doFill, fillByte))
    {
        table_load_state(table, LS_ERROR);
        table->error = E_MAX_TABLE_LENGTH_EXEEDED;
    }
}

static void table_load_event_loading(struct table_object *table, uint8_t *data)
{
    uint8_t event = data[0];
    switch (event)
    {
    case LE_NOOP:
    case LE_START_LOADING:
        break;
    case LE_LOAD_COMPLETED:
        table_load_state(table, LS_LOADED);
        break;
    case LE_UNLOAD:
        table_load_state(table, LS_UNLOADED);
        break;
    case LE_ADDITIONAL_LOAD_CONTROLS:
        table_additional_load_controls(table, data);
        break;
    default:
        table_load_state(table, LS_ERROR);
        table->error = E_GOT_UNDEF_LOAD_CMD;
    }
}

static void table_load_event_loaded(struct table_object *table, uint8_t *data)
{
    uint8_t event = data[0];
    switch (event)
    {
    case LE_NOOP:
    case LE_LOAD_COMPLETED:
        break;
    case LE_START_LOADING:
        table_load_state(table, LS_LOADING);
        break;
    case LE_UNLOAD:
        table_load_state(table, LS_UNLOADED);
        break;
    case LE_ADDITIONAL_LOAD_CONTROLS:
        table_load_state(table, LS_ERROR);
        table->error = E_INVALID_OPCODE;
        break;
    default:
        table_load_state(table, LS_ERROR);
        table->error = E_GOT_UNDEF_LOAD_CMD;
    }
}

static void table_load_event_error(struct table_object *table, uint8_t *data)
{
    uint8_t event = data[0];
    switch (event)
    {
    case LE_NOOP:
    case LE_LOAD_COMPLETED:
    case LE_ADDITIONAL_LOAD_CONTROLS:
    case LE_START_LOADING:
        break;
    case LE_UNLOAD:
        table_load_state(table, LS_UNLOADED);
        break;
    default:
        table_load_state(table, LS_ERROR);
        table->error = E_GOT_UNDEF_LOAD_CMD;
    }
}

static void table_load_event(struct table_object *table, uint8_t *data)
{
    switch (table->state)
    {
    case LS_UNLOADED:
        table_load_event_unloaded(table, data);
        break;
    case LS_LOADING:
        table_load_event_loading(table, data);
        break;
    case LS_LOADED:
        table_load_event_loaded(table, data);
        break;
    case LS_ERROR:
        table_load_event_error(table, data);
        break;
    default:
        /* do nothing */
        break;
    }
}

void table_write_property(struct table_object *table, PropertyID id, uint8_t start, uint8_t *data,
                          uint8_t count)
{
    switch (id)
    {
    case PID_LOAD_STATE_CONTROL:
        table_load_event(table, data);
        break;

    // case PID_MCB_TABLE:
    //    TODO
    //    break;
    default:
        break;
    }
}

uint8_t table_property_size(PropertyID id)
{
    switch (id)
    {
    case PID_LOAD_STATE_CONTROL:
        return 1;
    case PID_TABLE_REFERENCE:
        return 4;
    case PID_ERROR_CODE:
        return 1;
    case PID_OBJECT_TYPE:
        return 2;
    default:
        return 0;
    }
}
