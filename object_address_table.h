#ifndef __OBJECT_ADDRESS_TABLE__
#define __OBJECT_ADDRESS_TABLE__

#include "object_table.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t address_table_get_group_address(uint16_t tsap);
uint16_t address_table_get_tsap(uint16_t addr);
bool address_table_contains(uint16_t addr);
uint8_t address_table_property_count();
struct property_description *address_table_property_descriptions();
void init_address_table_object();
LoadState address_table_load_state();
uint8_t *address_table_save(uint8_t *buffer);

uint8_t *address_table_restore(uint8_t *buffer);

void address_table_read_property_description(uint8_t *propertyId, uint8_t *propertyIndex,
                                             bool *writeEnable, uint8_t *type,
                                             uint16_t *numberOfElements, uint8_t *access);
void address_table_read_property(PropertyID id, uint32_t start, uint32_t *count, uint8_t *data);
void address_table_write_property(PropertyID id, uint8_t start, uint8_t *data, uint8_t count);
uint8_t address_table_property_size(PropertyID id);

#ifdef __cplusplus
}
#endif

#endif
