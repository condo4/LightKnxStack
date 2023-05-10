/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __OBJECT_APPLICATION_PROGRAM__
#define __OBJECT_APPLICATION_PROGRAM__

#include "object_table.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t application_program_object_get_byte(uint32_t addr);
uint16_t application_program_object_get_word(uint32_t addr);
uint32_t application_program_object_get_int(uint32_t addr);
uint8_t *application_program_object_data(uint32_t addr);
LoadState application_program_object_load_state();
uint8_t *application_program_object_save(uint8_t *buffer);
uint8_t *application_program_object_restore(uint8_t *buffer);
void init_application_program();
void application_program_object_read_property_description(uint8_t *propertyId,
                                                          uint8_t *propertyIndex, bool *writeEnable,
                                                          uint8_t *type, uint16_t *numberOfElements,
                                                          uint8_t *access);
void application_program_object_read_property(PropertyID id, uint32_t start, uint32_t *count,
                                              uint8_t *data);
void application_program_object_write_property(PropertyID id, uint8_t start, uint8_t *data,
                                               uint8_t count);
uint8_t application_program_object_property_size(PropertyID id);

#ifdef __cplusplus
}
#endif

#endif
