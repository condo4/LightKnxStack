#pragma once

#include "object_table.h"
#include "object_group.h"

#if __cplusplus
extern "C" {
#endif

uint16_t group_object_table_data(uint16_t asap);

uint8_t *group_object_table_save(uint8_t *buffer);

uint8_t *group_object_table_restore(uint8_t *buffer);

void init_group_object_table();

void group_object_table_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                                  bool *writeEnable, uint8_t *type,
                                                  uint16_t *numberOfElements, uint8_t *access);
void group_object_table_read_property(PropertyID id, uint32_t start, uint32_t *count,
                                      uint8_t *data);
void group_object_table_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count);
uint8_t group_object_table_property_size(PropertyID id);
uint16_t group_object_table_entry_count();
struct group_object *group_object_table_get(uint16_t asap);

LoadState group_object_table_load_state();

#if __cplusplus
}
#endif
