#include "object_interface.h"

void interface_read_property_description(struct property_description *descriptions, uint8_t count,
                                         uint8_t *propertyId, uint8_t *propertyIndex,
                                         bool *writeEnable, uint8_t *type,
                                         uint16_t *numberOfElements, uint8_t *access)
{
    *numberOfElements = 0;
    if (descriptions == NULL || count == 0)
        return;

    struct property_description *desc = NULL;

    // from KNX spec. 03.03.07 Application Layer (page 56) - 3.4.3.3
    // A_PropertyDescription_Read-service Summary: either propertyId OR propertyIndex, but not both
    // at the same time
    if ((*propertyId) != 0)
    {
        for (uint8_t i = 0; i < count; i++)
        {
            struct property_description d = descriptions[i];
            if (d.Id != *propertyId)
                continue;

            desc = &d;
            *propertyIndex = i;
            break;
        }
    }
    else
    {
        // If propertyId is zero, propertyIndex shall be used.
        // Response: propertyIndex of received A_PropertyDescription_Read
        if (*propertyIndex < count)
        {
            desc = &descriptions[*propertyIndex];
        }
    }

    if (desc != NULL)
    {
        *propertyId = desc->Id;
        *writeEnable = desc->WriteEnable;
        *type = desc->Type;
        *numberOfElements = desc->MaxElements;
        *access = desc->Access;
    }
}
