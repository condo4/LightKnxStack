#ifndef __KNX_TYPES__
#define __KNX_TYPES__

#include <stdint.h>

#if !defined(SUPPORT_EXTENDED_FRAME_FORMAT)
#    define MAX_KNX_TELEGRAM_SIZE 22
#else
#    define MAX_KNX_TELEGRAM_SIZE 269
#endif

typedef enum
{
    Req_start_of_Frame,
    Req_inner_Frame_char,
    Req_ack_char,
    Req_poll_data_char,
    Req_fill_char
} Ph_Data_Req_Class;

typedef enum
{
    Con_OK,
    Con_bus_not_free,
    Con_collision_detected,
    Con_transceiver_fault
} Ph_Data_Con_Class;

typedef enum
{
    Ind_start_of_Frame,
    Ind_inner_Frame_char,
    Ind_ack_char,
    Ind_poll_data_char,
    Ind_parity_error,
    Ind_framing_error,
    Ind_bit_error
} Ph_Data_Ind_Class;

typedef enum
{
    L_Data_Extended = 0x0,
    L_Data_Standard = 0x1
} FrameFormat;

typedef enum
{
    LowPriority = 0x3, /* Normal priority of group communication */
    NormalPriority = 0x1, /* More important telegrams like central functions */
    UrgentPriority = 0x2, /* Used for alarms */
    SystemPriority = 0x0, /* Mainly used by ETS for device programming */
    Invalid = 0x4, /* Used for transmition */
    Repeted = 0x5 /* Used for transmition */
} Priority;

typedef enum
{
    Individual = 0x0,
    Group = 0x1
} AddressType;
#define Multicast Group

typedef uint16_t KnxAddress;

typedef enum
{
    l_ok = 0x0,
    l_not_ok = 0x1
} L_Status;

typedef enum
{
    n_ok = 0x0,
    n_not_ok = 0x1
} N_Status;

enum HopCountType
{
    NetworkLayerParameter, /* use N_hop_count */
    UnlimitedRouting = 7 /* hopCount is set to 7. This means that the frame never expires. This
                            could be a problem if your bus contains a circle. */
};

typedef enum
{
    t_ok = 0x0,
    t_not_ok = 0x1
} T_Status;

typedef enum
{
    p_ok = 0x0,
    p_transceiver_fault = 0x1,
    p_collision_detected = 0x2,
    P_bus_not_free = 0x3,
} P_Status;

typedef enum
{
    a_ok = 0x0,
    a_not_ok = 0x1
} A_Status;

#endif
