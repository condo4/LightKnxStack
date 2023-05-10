/*
 * Project:   LightKnxStack
 * Author:    Fabien Proriol
 * Created:   2023.05.10
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */


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
    a_ok = 0x0,
    a_not_ok = 0x1
} A_Status;

#endif
