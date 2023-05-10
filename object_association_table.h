/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __OBJECT_ASSOCIATION_TABLE__
#define __OBJECT_ASSOCIATION_TABLE__

#include "object_table.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t association_table_next_asap(uint16_t tsap, uint16_t *startIdx);
int32_t association_table_translate_asap(uint16_t asap);
uint8_t *association_table_save(uint8_t *buffer);
uint8_t *association_table_restore(uint8_t *buffer);
void init_association_table();
void association_table_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                                 bool *writeEnable, uint8_t *type,
                                                 uint16_t *numberOfElements, uint8_t *access);
void association_table_read_property(PropertyID id, uint32_t start, uint32_t *count, uint8_t *data);
void association_table_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count);
uint8_t association_table_property_size(PropertyID id);
LoadState association_table_load_state();

#ifdef __cplusplus
}
#endif

#endif
