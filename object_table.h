#ifndef __OBJECT_TABLE__
#define __OBJECT_TABLE__

#include "object_property_types.h"

/**
 * container_of - cast a member of a structure out to the containing structure
 * \param ptr:	the pointer to the member.
 * \param type:	the type of the container struct this is embedded in.
 * \param member:	the name of the member within the struct.
 */
#define container_of(ptr, type, member)                                                            \
    ({                                                                                             \
        const typeof(((type *)0)->member) *__mptr = (ptr);                                         \
        (type *)((char *)__mptr - offsetof(type, member));                                         \
    })

struct table_object
{
    void (*before_state_change)(struct table_object *table, LoadState *newState);
    volatile LoadState state;
    uint8_t *data;
    volatile uint32_t size;
    volatile ErrorCode error;
};

#ifdef __cplusplus
extern "C" {
#endif

void init_table_object(struct table_object *table);
void table_read_property(struct table_object *table, PropertyID id, uint32_t start, uint32_t *count,
                         uint8_t *data);
void table_write_property(struct table_object *table, PropertyID id, uint8_t start, uint8_t *data,
                          uint8_t count);
void table_load_state(struct table_object *table, LoadState newState);
uint8_t table_property_size(PropertyID id);
uint8_t *table_object_save(struct table_object *table, uint8_t *buffer);
uint8_t *table_object_restore(struct table_object *table, uint8_t *buffer);

#ifdef __cplusplus
}
#endif

#endif
