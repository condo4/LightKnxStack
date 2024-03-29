/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __OBJECT_DEVICE__
#define __OBJECT_DEVICE__

#include "object_interface.h"
#include <platform.h>

void device_induvidual_address_set(uint16_t value);
bool device_induvidual_address_duplication();
void device_induvidual_address_duplication_set(bool value);

uint16_t device_manufacturer_id();
void device_manufacturer_id_set(uint16_t value);

uint32_t device_bau_number();
void device_bau_number_set(uint32_t value);

const uint32_t *device_if_obj();
void device_if_obj_set(const uint32_t *value);

uint16_t device_mask_version();
void device_mask_version_set(uint16_t value);

bool device_verify_mode();
void device_verify_mode_set(bool value);

bool device_user_stopped();
void device_user_stopped_set(bool value);

bool device_safe_state();
void device_safe_state_set(bool value);

const char *device_order_number();
void device_order_number_set(const char *value);

const uint8_t *device_hardware_type();
void device_hardware_type_set(const uint8_t *value);

uint16_t device_version();
void device_version_set(uint16_t value);

uint16_t device_max_apdu_length();
void device_max_apdu_length_set(uint16_t value);

/* Storage */
uint8_t *device_object_save(uint8_t *buffer);
uint8_t *device_object_restore(uint8_t *buffer);

/* Property */
void device_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                      bool *writeEnable, uint8_t *type, uint16_t *numberOfElements,
                                      uint8_t *access);
void device_read_property(PropertyID propertyId, uint32_t start, uint32_t *count, uint8_t *data);
void device_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count);
uint8_t device_object_property_size(PropertyID id);


#endif
