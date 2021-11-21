#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "console.h"
//#define DEBUG_STKNX_READ
//#define DEBUG_STKNX_WRITE
#define DEBUG_LAYER_PHYSICAL
#define DEBUG_LAYER_LOW_DATA_LINK
#define DEBUG_LAYER_DATA_LINK
#define DEBUG_LAYER_NETWORK
#define DEBUG_LAYER_TRANSPORT
#define DEBUG_LAYER_APPLICATION

#if defined(DEBUG_STKNX_READ) || defined(DEBUG_STKNX_WRITE) || defined(DEBUG_LAYER_LOW_DATA_LINK)  \
        || defined(DEBUG_LAYER_DATA_LINK) || defined(DEBUG_LAYER_NETWORK)                          \
        || defined(DEBUG_LAYER_TRANSPORT) || defined(DEBUG_LAYER_APPLICATION)

#    include "console.h"
#    include "knx_types.h"

static inline void print_source_address(KnxAddress src)
{
    console_print_int(src >> 12);
    console_print_char('.');
    console_print_int((src >> 8) & 0x0F);
    console_print_char('.');
    console_print_int(src & 0xFF);
}
#define print_dest_indiv_address(dst) print_source_address(dst)

static inline void print_dest_group_address(KnxAddress dst)
{
    console_print_int(dst >> 11);
    console_print_char('/');
    console_print_int((dst >> 8) & 0x07);
    console_print_char('/');
    console_print_int(dst & 0xFF);
}

static inline void print_priority(Priority priority)
{
    switch (priority)
    {
    case LowPriority:
        console_print_string("low");
        break;
    case NormalPriority:
        console_print_string("normal");
        break;
    case UrgentPriority:
        console_print_string("urgent");
        break;
    case SystemPriority:
        console_print_string("system");
        break;
    default:
        console_print_string("UNKNOWN");
        break;
    }
}

static inline void print_address_type(AddressType type)
{
    switch (type)
    {
    case Individual:
        console_print_string("Individual");
        break;
    case Group:
        console_print_string("Group");
        break;

    default:
        console_print_string("UNKNOWN");
        break;
    }
}

static inline void print_frame_format(FrameFormat format)
{
    switch (format)
    {
    case L_Data_Extended:
        console_print_string("L_Data_Extended");
        break;
    case L_Data_Standard:
        console_print_string("L_Data_Standard");
        break;

    default:
        console_print_string("UNKNOWN");
        break;
    }
}

#endif /* DEBUG */

#endif
