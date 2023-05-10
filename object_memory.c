/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "object_memory.h"
#include "object_device.h"
#include "object_application_program.h"
#include "object_address_table.h"
#include "object_association_table.h"
#include "object_group_table.h"
#include <platform.h>
#include <stdlib.h>
#include <cmsis_gcc.h>
#include <console.h>

static bool _modified = false;
static uint8_t* _data = 0;

void memory_modified()
{
    _modified = true;
}


void memory_read()
{
    _data = platform_eeprom_get_buffer(512);

    if (_data[0] != 0x00 || _data[1] != 0xAD || _data[2] != 0xAF || _data[3] != 0xFE)
        return;

    uint8_t* buffer = _data + 4;
    buffer = device_object_restore(buffer);
    //buffer = application_program_object_restore(buffer);
    //buffer = address_table_restore(buffer);
    //buffer = association_table_restore(buffer);
    //buffer = group_object_table_restore(buffer);
}



void memory_write()
{
    _data[0] = 0x00;
    _data[1] = 0xAD;
    _data[2] = 0xAF;
    _data[3] = 0xFE;

    uint8_t* buffer = _data + 4;
    buffer = device_object_save(buffer);
    //buffer = application_program_object_save(buffer);
    //buffer = address_table_save(buffer);
    //buffer = association_table_save(buffer);
    //buffer = group_object_table_save(buffer);

    platform_eeprom_commit();
    _modified = false;
}

