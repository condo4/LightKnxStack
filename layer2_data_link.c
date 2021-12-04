#include "layer2_data_link.h"
#include "layer1_physical.h"
#include "layer3_network.h"
#include "object_device.h"
#include "object_address_table.h"
#include "debug.h"

/* This layer must be the last layer synchronious since it control the ACK */

static volatile uint8_t TX[MAX_KNX_TELEGRAM_SIZE] = {
    0,
};
static uint8_t tx_busy = 0;
static uint8_t tx_index = 0;
static uint8_t hop_count_type = 6;



static enum N_Mode {
    None,
    Data_Individual,
    Data_Group,
    Data_Broadcast,
    Data_SystemBroadcast
} _mode = None;

static KnxAddress _destination_address;
static uint8_t _hop_count_type;
static uint8_t _octet_count;
static Priority _priority;
static KnxAddress _source_address;
static uint8_t *_nsdu;


void L_Data__req(uint8_t ack_request, AddressType address_type, KnxAddress destination_address,
                 FrameFormat frame_format, uint8_t *lsdu, uint8_t octet_count, Priority priority,
                 KnxAddress source_address)
{
    int i;
    uint8_t crc = 0;

    if (tx_busy)
    {
        console_print_string("ERROR, L_Data__req BUSY\r\n");
        return;
    }
    // tx_busy = 1;

    /* Control Byte */
    TX[0] = 0x30; /* L_Data-Frame */
    TX[0] |= ((priority << 2) & 0x0C);
    TX[0] |= ((frame_format << 7) & 0x80);

    /* Source address */
    TX[1] = (source_address >> 8) & 0xFF;
    TX[2] = (source_address >> 0) & 0xFF;

    /* Destination Address */
    TX[3] = (destination_address >> 8) & 0xFF;
    TX[4] = (destination_address >> 0) & 0xFF;

    /* AT NPCI LG */
    TX[5] = (address_type << 7) | (hop_count_type << 4) | (octet_count - 1);

    /* Data */
    for (i = 0; i < octet_count; i++)
    {
        TX[6 + i] = lsdu[i];
    }

    /* Checksum */
    for (i = 0; i < 6 + octet_count; i++)
        crc ^= TX[i];
    TX[6 + octet_count] = ~crc;

#ifdef DEBUG_LAYER_LOW_DATA_LINK
    console_print_string("<--- ");
    console_print_bytes(TX, octet_count + 7);
    console_print_string("\r\n");
#endif

    /* For transmit, don't follow Exactelly Ph_Data Service as describe in documention, I find
     * inadapted to transmition by timer */
    Ph_Knx_Start_Transmit(priority, &TX[0], octet_count + 7);
}

void L_Data__con(AddressType address_type, KnxAddress destination_address, FrameFormat frame_format,
                 uint8_t octet_count, Priority priority, KnxAddress source_address, uint8_t *lsdu,
                 L_Status l_status)
{
    if (l_status == l_ok)
    {
        /* Send next byte */
        if (tx_index > octet_count)
        {
            /* Sent full frame successfully */
            tx_busy = 0;
            tx_index = 0;
            return;
        }
        Ph_Data__req(Req_inner_Frame_char, TX[tx_index++]);
    }
    else
    {
        /* Probably bus busy, retry later */
        tx_index = 0;
    }
}

#define LSDU_HOP_COUNT(lsdu) ((lsdu[0] >> 4) & 0x7)

void L_Data__ind(uint8_t ack_request, AddressType address_type, KnxAddress destination_address,
                 FrameFormat frame_format, uint8_t *lsdu, uint8_t octet_count, Priority priority,
                 KnxAddress source_address)
{
#if defined(DEBUG_LAYER_DATA_LINK)
    console_print_string("L_Data__ind(");
    console_print_int(ack_request);
    console_print_string(", ");
    print_address_type(address_type);
    console_print_string(", ");
    if(address_type == Individual) print_dest_indiv_address(destination_address);
    else print_dest_group_address(destination_address);
    console_print_string(", ");
    print_frame_format(frame_format);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(lsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(lsdu[octet_count - 1]);
    console_print_string("], ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(")\r\n");
#endif

    if(_mode != None)
    {
        // TIMING ERROR !!!!
        return;
    }
    _destination_address = destination_address;
    _hop_count_type = LSDU_HOP_COUNT(lsdu);
    _octet_count = octet_count - 1;
    _priority = priority;
    _source_address = source_address;
    _nsdu = lsdu + 1;

    if (frame_format == L_Data_Standard)
    {
        if (address_type == Individual)
        {
            /*
             * The remote Network Layer shall map an L_Data.ind primitive with address_type =
             * ’individual’ to an N_Data_Individual.ind primitive. It shall remove the hop_count and
             * shall generate the parameter hop_count_type according to its value. The argument lsdu
             * shall be decoded to the argument nsdu. The arguments octet_count, priority and
             * source_address shall be mapped to the corresponding arguments octet_count, priority
             * and source_address of the N_Data_Individual.ind primitive.
             */
            if (destination_address == device_induvidual_address())
            {
                if (ack_request)
                {
                    /* Send back ACK here */
                    Ph_Data__req(Req_ack_char, FRAME_ACK);
                }
                _mode = Data_Individual;
            }
        }
        else /* Multicast */
        {
            if (destination_address != 0)
            {
                /*
                 * The remote Network Layer shall map an L_Data.ind primitive with address_type =
                 * ‘multicast’ and destination_address<>‘0’ to an N_Data_Group.ind primitive. It
                 * shall remove the hop_count and generates the parameter hop_count_type according
                 * to its value. The arguments destination_address, octet_count and priority are
                 * mapped to the corresponding arguments destination_address, octet_count and
                 * priority of the N_Data_Group.ind primitive.
                 */
                if (address_table_get_tsap(destination_address) != 0xFFFF)
                {
                    if (ack_request)
                    {
                        /* Send back ACK here */
                        Ph_Data__req(Req_ack_char, FRAME_ACK);
                    }
                    _hop_count_type = (LSDU_HOP_COUNT(lsdu) == 7)
                                            ? (UnlimitedRouting)
                                            : (NetworkLayerParameter);

                    _mode = Data_Group;
                }
            }
            else
            {
                /*
                 * The remote Network Layer shall map an L_Data.ind primitive with address_type =
                 * ́multicast ́ and destination_address =  ́0 ́ to an N_Data_Broadcast.ind primitive. It
                 * shall remove the hop_count and shall generate the parameter hop_count_type
                 * according to its value. The argument lsdu shall be mapped to the argument nsdu.
                 * The argument priority shall be mapped to the corresponding argument priority of
                 * the N_Data_Broadcast.ind primitive.
                 */
                if (ack_request)
                {
                    /* Send back ACK here */
                    Ph_Data__req(Req_ack_char, FRAME_ACK);
                }

                _mode = Data_Broadcast;
            }
        }
    }
}

void L_SystemBroadcast__ind(uint8_t ack_request, AddressType address_type,
                            KnxAddress destination_address, uint8_t *lsdu, uint8_t octet_count,
                            Priority priority, KnxAddress source_address)
{
#if defined(DEBUG_LAYER_DATA_LINK)
    console_print_string("L_SystemBroadcast__ind(");
    console_print_int(ack_request);
    console_print_string(", ");
    print_address_type(address_type);
    console_print_string(", ");
    print_dest_group_address(destination_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(lsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(lsdu[octet_count - 1]);
    console_print_string("], ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(")\r\n");
#endif

    _destination_address = destination_address;
    _hop_count_type = LSDU_HOP_COUNT(lsdu);
    _octet_count = octet_count - 1;
    _priority = priority;
    _source_address = source_address;
    _nsdu = lsdu + 1;
    _mode = Data_SystemBroadcast;
}

typedef enum
{
    L_STATE_POWER_ON,
    L_STATE_WAIT_FOR_RESET,
    L_STATE_NOMAL_MODE_IDLE,
    L_STATE_BUS_MONITOR_MODE_IDLE,
    L_STATE_STOP
} L_State;

static L_State l_state = L_STATE_POWER_ON;

void L_Loop()
{
    switch (l_state)
    {
    case L_STATE_POWER_ON:
        l_state = L_STATE_WAIT_FOR_RESET;
        Ph_Reset__req();
        break;
    case L_STATE_NOMAL_MODE_IDLE:
        switch(_mode)
        {
            case None:
                break;
            case Data_Individual:
                N_Data_Individual__ind(
                    _destination_address,
                    _hop_count_type,
                    _octet_count,
                    _priority,
                    _source_address,
                    _nsdu);
                break;
            case Data_Group:
                N_Data_Group__ind(
                    _destination_address,
                    _hop_count_type,
                    _octet_count,
                    _priority,
                    _nsdu,
                    _source_address);
                break;
            case Data_Broadcast:
                N_Data_Broadcast__ind(
                    _hop_count_type,
                    _octet_count,
                    _priority,
                    _source_address,
                    _nsdu);
                break;
            case Data_SystemBroadcast:
                N_Data_SystemBroadcast__ind(
                    _hop_count_type,
                    _nsdu,
                    _octet_count,
                    _priority,
                    _source_address);
                break;
        }
        _mode = None;
        break;
    default:
        break;
    }
}

void Ph_Reset__con(P_Status status)
{
    if (status == p_ok)
    {
        if (l_state == L_STATE_WAIT_FOR_RESET)
        {
            l_state = L_STATE_NOMAL_MODE_IDLE;
        }
    }
}
