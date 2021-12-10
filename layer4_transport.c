#include "layer4_transport.h"
#include "layer7_application.h"
#include "layer3_network.h"
#include "object_association_table.h"
#include "object_device.h"
#include "debug.h"

extern uint32_t millis();

/* State Machine of Connection-Oriented Communication Mode */
enum StateType {
    CLOSED,     /* There is no connection. */
    OPEN_IDLE,  /* There is a connection open. */
    OPEN_WAIT,  /* The state machine is waiting for a T_ACK when data has been sent to the remote partner */
    CONNECTING  /* Client only. The state is waiting for an IACK after trying to connect to aremote partner. */
};
static KnxAddress _connection_address;
static uint8_t _seqNoSend;
static uint8_t _seqNoRcv;
static int _connection_timeout_timer = 0;
static int _acknowledgment_timeout_timer;
static int _rep_count;
static int _current_state = CLOSED;

#define MAX_REP_COUNT 3

#define NOT_USED(x) ( (void)(x) )

/*
 * Do nothing
 */
void A0(KnxAddress source, Priority priority, uint8_t *data)
{
    NOT_USED(source);
    NOT_USED(priority);
    NOT_USED(data);
}

/*
 * Connection_address = source address of received message
 * Send a T_CONNECT_ind to the user.
 * SeqNoSend=0; SeqNoRcv =0;
 * Start connection timeout timer
 */
void A1(KnxAddress source, Priority priority, uint8_t *data)
{
    _connection_address = source;
    A_Connect__ind(source);
    _seqNoSend = 0;
    _seqNoRcv = 0;
    _connection_timeout_timer = millis();
    _current_state = OPEN_IDLE;
}

/*
 * Send a N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination =
 * connection_address, sequence = SeqNoRcv to the Network Layer (remote device).
 * Increment the SeqNoRcv.
 * Send the received buffer as a T_Data_Connected.ind to the user.
 * Restart the connection timeout timer.
 */
void A2(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO send_control_telegram(Ack, _seqNoRecv);
    _seqNoRcv += 1;
    if(_seqNoRcv > 0xf)
        _seqNoRcv = 0;
    // TODO application_data_connected_indication(priority, source, apdu);
    // TODO enable_connection_timeout();
}

/*
 * Send an N_Data_Individual.req with T_ACK_PDU, priority = SYSTEM, destination =
 * connection_address, sequence = sequence of received message to the Network Layer
 * (remote device).
 * Restart the connection timeout timer.
 */
void A3(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO send_control_telegram(Ack, recTpdu.sequenceNumber());
    // TODO enable_connection_timeout();
}

/*
 * Send an N_Data_Individual.req with T_NAK_PDU, priority = SYSTEM, destination =
 * connection_address, sequence = sequence of received message to the Network Layer
 * (remote device).
 * Restart the connection timeout timer.
 */
void A4(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO send_control_telegram(Nack, recTpdu.sequenceNumber());
    // TODO enable_connection_timeout();
}

/*
 * Send a T_Disconnect.ind to the user.
 * Stop the acknowledge timeout timer.
 * Stop the connection timeout timer.
 */
void A5(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO application_disconnect_indication(tsap);
    // TODO disable_connection_timeout();
    // TODO disable_ack_timeout();
    _current_state = CLOSED;
}

/*
 * Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM,
 * destination = connection_address, sequence = 0 to the Network Layer (remote device).
 * Send a T_Disconnect.ind to the user.
 * Stop the acknowledge timeout timer.
 * Stop the connection timeout timer.
 */
void A6(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO send_control_telegram(Disconnect, 0);
    // TODO application_disconnect_indication(tsap);
    // TODO disable_connection_timeout();
    // TODO disable_ack_timeout();
    _current_state = CLOSED;
}

/*
 * Store the received T_Data_Connected.req and send as a N_Data_Individual.req with
 * T_DATA_CONNECTED_REQ_PDU, destination = connection_address, sequence =
 * SeqNoSend to the Network Layer (remote device).
 * Clear the rep_count.
 * Start the acknowledge timeout timer.
 * Restart the connection timeout timer.
 */
void A7(KnxAddress source, Priority priority, uint8_t *data)
{
    /* TODO
    _savedPriority = priority;
    TPDU& tpdu = apdu.frame().tpdu();
    tpdu.type(DataConnected);
    tpdu.sequenceNumber(_seqNoSend);
    _savedFrame = apdu.frame();
    N_Data_Individual__request(AckRequested, _connectionAddress, NetworkLayerParameter, priority, tpdu);
    _repCount = 0;
    enable_ack_timeout();
    enable_connection_timeout();
    */
    _current_state = OPEN_WAIT;
}

#if !defined(STYLE_2)
/*
 * Stop the acknowledge timeout timer.
 * Increment the SeqNoSend.
 * Send the stored buffer as a T_Data_Connected.con with cleared errorbits,connection
 * number = 0 to the user.
 * Restart the connection timeout timer.
 */
void A8(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO disable_ack_timeout();
    _seqNoSend += 1;
    if(_seqNoSend > 0xf)
        _seqNoSend = 0;
    // TODO enable_connection_timeout();
    _current_state = OPEN_IDLE;
}

#else
/*
 * Stop the acknowledge timeout timer.
 * Increment the SeqNoSend.
 * Restart the connection timeout timer.
 */
void A8b(KnxAddress source, Priority priority, uint8_t *data)
{

    _current_state = OPEN_IDLE;
}
#endif

/*
 * Send the stored message as a N_Data_Individual.req to the Network Layer (remote
 * device).
 * Increment the rep_count.
 * Start the acknowledge timeout timer.
 * Restart the connection timeout timer.
 */
void A9(KnxAddress source, Priority priority, uint8_t *data)
{
    // TODO TPDU& tpdu = _savedFrame.tpdu();
    // tpdu is still initialized from last send
    // TODO N_Data_Individual__request(AckRequested, _connectionAddress, NetworkLayerParameter, _savedPriority, tpdu);
    _rep_count += 1;
    // TODO enable_ack_timeout();
    // TODO enable_connection_timeout();
}

/*
 * Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU Priority = SYSTEM,
 * Destination = source (rbuffer), Sequence = 0 back to sender.
 */
void A10(KnxAddress source, Priority priority, uint8_t *data)
{
    /*
    CemiFrame frame;
    TPDU& tpdu = frame.tpdu();
    tpdu.type(Disconnect);
    tpdu.sequenceNumber(0);
    N_Data_Individual__request(AckRequested, source, NetworkLayerParameter, SystemPriority, tpdu);
    */
}

/*
 * Store event back and handle after next event. Don’t change order of
 * T_Data_Connected.req events
 */
void A11(KnxAddress source, Priority priority, uint8_t *data)
{
    /*
    _savedTsapConnecting = tsap;
    _savedPriorityConnecting = priority;
    _savedFrameConnecting = apdu.frame();
    _savedConnectingValid = true;
    */
}

/*
 * connection_address=address from T_CONNECT_requ
 * send N_Data_Individual.req with T_CONNECT_REQ_PDU
 * SeqNoSend=0; SeqNoRcv =0;
 * Start connection timeout timer
 */
void A12(KnxAddress source, Priority priority, uint8_t *data)
{
    /*
    _connectionAddress = destination;
    CemiFrame frame(0);
    TPDU& tpdu = frame.tpdu();
    tpdu.type(Connect);
    N_Data_Individual__request(AckRequested, destination, NetworkLayerParameter, priority, tpdu);
    _seqNoRecv = 0;
    _seqNoSend = 0;
    enable_connection_timeout();
    */
    _current_state = OPEN_IDLE;
}

/*
 * Send a T_Connect.con to the user.
 */
void A13(KnxAddress source, Priority priority, uint8_t *data)
{
//    application_connect_confirm(destination, 0, true);
}

#if !defined(STYLE_2)
/*
 * Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM,
 * destination = connection_address, sequence = 0 to the Network Layer (remote device).
 * Send a T_Disconnect.con to the user.
 * Stop the acknowledge timeout timer.
 * Stop the connection timeout timer.
 */
void A14(KnxAddress source, Priority priority, uint8_t *data)
{
    /*
    CemiFrame frame(0);
    TPDU& tpdu = frame.tpdu();
    tpdu.type(Disconnect);
    tpdu.sequenceNumber(0);
    N_Data_Individual__request(AckRequested, _connectionAddress, NetworkLayerParameter, SystemPriority, tpdu);
    application_disconnect_confirm(priority, tsap, true);
    disable_connection_timeout();
    disable_ack_timeout();
    */
    _current_state = CLOSED;
}
#else
/*
 * Send a N_Data_Individual.req with T_DISCONNECT_REQ_PDU, priority = SYSTEM,
 * destination = connection_address, sequence = 0 to the Network Layer (remote device).
 * Stop the acknowledge timeout timer.
 * Stop the connection timeout timer.
 */
void A14b(KnxAddress source, Priority priority, uint8_t *data)
{
    _current_state = CLOSED;
}
#endif

/*
 * Send a T_Disconnect.con
 * to the management user
 * Stop the acknowledge timeout timer.
 * Stop the connection timeout timer.
 */
void A15(KnxAddress source, Priority priority, uint8_t *data)
{
/*
 *     application_disconnect_confirm(priority, tsap, true);
    disable_connection_timeout();
    disable_ack_timeout();
    */
    _current_state = CLOSED;
}

#if defined(STYLE_3)
void (*_state_machine[28][4])(KnxAddress source, Priority priority, uint8_t *data) = {
                                    {A1,     A0,     A0,     A0},     // E00
                                    {A1,     A10,    A10,    A10},    // E01
                                    {A0,     A5,     A5,     A5},     // E02
                                    {A0,     A0,     A0,     A0},     // E03
                                    {A0,     A2,     A2,     A6},     // E04
                                    {A0,     A3,     A3,     A3},     // E05
                                    {A0,     A4,     A4,     A6},     // E06
                                    {A0,     A0,     A0,     A10},    // E07
                                    {A0,     A0,     A8,     A6},     // E08
                                    {A0,     A0,     A6,     A6},     // E09
                                    {A0,     A0,     A0,     A10},    // E10
                                    {A0,     A0,     A0,     A6},     // E11
                                    {A0,     A6,     A9,     A6},     // E12
                                    {A0,     A6,     A9,     A6},     // E13
                                    {A0,     A0,     A0,     A10},    // E14
                                    {A0,     A7,     A11,    A11},    // E15
                                    {A0,     A6,     A6,     A6},     // E16
                                    {A0,     A0,     A9,     A0},     // E17
                                    {A0,     A0,     A6,     A0},     // E18
                                    {A0,     A0,     A0,     A13},    // E19
                                    {A0,     A0,     A0,     A5},     // E20
                                    {A0,     A0,     A0,     A0},     // E21
                                    {A0,     A0,     A0,     A0},     // E22
                                    {A0,     A0,     A0,     A0},     // E23
                                    {A0,     A0,     A0,     A0},     // E24
                                    {A12,    A6,     A6,     A6},     // E25
                                    {A15,    A14,    A14,    A14},    // E26
                                    {A0,     A0,     A0,     A0},     // E27
};
#else
/* Style 1 rationalised */
void (*_state_machine[28][4])(KnxAddress source, Priority priority, uint8_t *data0) = {
                                    {A1,     A6,     A6,     A0},     // E00
                                    {A1,     A10,    A10,    A0},     // E01
                                    {A0,     A5,     A5,     A0},     // E02
                                    {A0,     A0,     A0,     A0},     // E03
                                    {A10,    A2,     A2,     A0},     // E04
                                    {A10,    A3,     A3,     A0},     // E05
                                    {A10,    A6,     A6,     A0},     // E06
                                    {A10,    A10,    A10,    A0},     // E07
                                    {A10,    A6,     A8,     A0},     // E08
                                    {A10,    A6,     A6,     A0},     // E09
                                    {A10,    A10,    A10,    A0},     // E10
                                    {A10,    A6,     A6,     A0},     // E11b
                                    {A0,     A0,     A0,     A0},     // E12 Event does not Exist
                                    {A0,     A0,     A0,     A0},     // E13 Event does not Exist
                                    {A10,    A10,    A10,    A0},     // E14
                                    {A5,     A7,     A11,    A0},     // E15
                                    {A0,     A6,     A6,     A0},     // E16
                                    {A0,     A0,     A0,     A0},     // E17 Event does not Exist
                                    {A0,     A0,     A0,     A0},     // E18 Event does not Exist
                                    {A0,     A13,    A13,    A0},     // E19
                                    {A0,     A5,     A5,     A0},     // E20
                                    {A0,     A0,     A0,     A0},     // E21
                                    {A0,     A0,     A0,     A0},     // E22
                                    {A0,     A0,     A0,     A0},     // E23
                                    {A0,     A0,     A0,     A0},     // E24 Event does not Exist
                                    {A12,    A6,     A6,     A0},     // E25
                                    {A15,    A14,    A14,    A0},     // E26
                                    {A0,     A0,     A0,     A0},     // E27
};
#endif

#define RUN_EVENT(num, source, priority, data) (_state_machine[num][_current_state])(source, priority, data)

#define T_SEQ_NO_OF_PDU(pdu)    ((pdu & 0x3C) >> 2)
#define T_MASK_PDU              0xC3
#define T_Data_Individual_PDU   0x00
#define T_Data_Connected_PDU    0x40
#define T_Connect_PDU           0x80
#define T_Disconnect_PDU        0x81
#define T_ACK_PDU               0xC2
#define T_NACK_PDU              0xC3



static void connection_timeout_indication()
{
    RUN_EVENT(16, 0, 0, 0);
}

static void ack_timeout_indication()
{
    if (_rep_count < MAX_REP_COUNT)
    {
        RUN_EVENT(17, 0, 0, 0);
    }
    else
    {
        RUN_EVENT(18, 0, 0, 0);
    }
}

/**
 * \fn T_Data_Group.ind(hop_count_type, octet_count, priority, source_address, TSAP, tsdu)
 * \param hop_count_type: hop count equals 7 or not
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: system, urgent, normal or low priority
 * \param source_address: the Individual Address of the originator of the message
 * \param TSAP: identifier of the service access point
 * \param tsdu: this is the user data that has been transferred by Transport Layer
 */
void T_Data_Group__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                       KnxAddress source_address, uint8_t TSAP, uint8_t *tsdu)
{
    uint16_t index = 0;
    int32_t asap = 0;
    uint16_t APCI = (tsdu[0] << 8 | tsdu[1]);

#if defined(DEBUG_LAYER_TRANSPORT)
    console_print_string("T_Data_Group__ind(");
    console_print_int(hop_count_type);
    console_print_string(", ");
    console_print_int(octet_count);
    console_print_string(", ");
    print_priority(priority);
    console_print_string(", ");
    print_source_address(source_address);
    console_print_string(", ");
    console_print_int(TSAP);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("]); APCI=");
    console_print_int(APCI);
    console_print_string("\r\n");
#endif

    do
    {
        asap = association_table_next_asap(TSAP, &index);
        if (asap == -1)
            return;

        switch (APCI)
        {
        case A_GroupValue_Read:
            A_GroupValue_Read__ind(asap, priority, hop_count_type);
            break;
        case A_GroupValue_Response:
            A_GroupValue_Read__Acon(asap, priority, hop_count_type, &tsdu[1], octet_count - 1);
            break;
        case A_GroupValue_Write:
            A_GroupValue_Write__ind(asap, priority, hop_count_type, &tsdu[1], octet_count - 1);
            break;
        default:
            console_print_string("WARNING: Unknonw 0x");
            console_print_hex(APCI);
            console_print_char('\r');
            console_print_char('\n');
            break;
        }
    } while (asap != -1);
}


enum
{
    DataBroadcast = 0,
    DataGroup,
    DataInduvidual,
    DataConnected,
    Connect,
    Disconnect,
    Ack,
    Nack,
};

uint8_t pdu_type(uint8_t type)
{
    if ((type & 0x80) > 0)
    {
        if ((type & 0x40) > 0)
        {
            if ((type & 1) == 0)
                return Ack;
            else
                return Nack;
        }
        else if ((type & 1) == 0)
            return Connect;
        else
            return Disconnect;
    }
    else
    {
        if ((type & 0x40) > 0)
            return DataConnected;
        else
            return DataInduvidual;
    }
}

/**
 * \fn T_Data_Individual.ind(hop_count_type, octet_count, priority, TSAP, tsdu)
 * \param hop_count_type: hop count equals 7 or not
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point (may be direct the Individual Address of the
 * remote partner) \param tsdu: is the user data that has been transferred by Transport Layer
 */
void T_Data_Individual__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                            KnxAddress source_address, uint8_t *tsdu)
{
#ifdef DEBUG_LAYER_TRANSPORT
    console_print_string("T_Data_Individual__ind(");
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_int(octet_count);
    console_print_char(',');
    print_priority(priority);
    console_print_char(',');
    print_source_address(source_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("]\r\n");
#endif

    switch(pdu_type(tsdu[0])) {
        case DataInduvidual:
        {
            // TODO: application_data_individual_indication(hopType, priority, source, tpdu.apdu());
            break;
        }
        case DataConnected:
        {
            if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) == _seqNoRcv)
            )
            {
                RUN_EVENT(4, source_address, priority, tsdu);
            }
            else if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) == ((_seqNoRcv - 1) & 0xF))
            )
            {
                RUN_EVENT(5, source_address, priority, tsdu);
            }
            else if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) != ((_seqNoRcv - 1) & 0xF)) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) != _seqNoRcv)
            )
            {
#if defined(STYLE_3)
                int state = _current_state;
                RUN_EVENT(6, source_address, priority, tsdu);
                if(state == CONNECTING)
                    _current_state = state;
#else
                RUN_EVENT(6, source_address, priority, tsdu);
#endif

            }
            else if(source_address != _connection_address)
            {
                RUN_EVENT(7, source_address, priority, tsdu);
            }
            break;
        }
        case Connect:
        {
            if(source_address == _connection_address)
                RUN_EVENT(0, source_address, priority, tsdu);
            else
                RUN_EVENT(1, source_address, priority, tsdu);
            break;
        }
        case Disconnect:
        {
            if(source_address == _connection_address)
                RUN_EVENT(2, source_address, priority, tsdu);
            else
                RUN_EVENT(3, source_address, priority, tsdu);
            break;
        }
        case Ack:
        {
            if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) == _seqNoRcv)
            )
            {
                RUN_EVENT(8, source_address, priority, tsdu);
            }
            else if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) != _seqNoRcv)
            )
            {
                RUN_EVENT(9, source_address, priority, tsdu);
            }
            else if(source_address != _connection_address)
            {
                RUN_EVENT(10, source_address, priority, tsdu);
            }
            break;
        }
        case Nack:
        {
            if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) != _seqNoRcv)
            )
            {
                RUN_EVENT(11, source_address, priority, tsdu);
            }
            else if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) == _seqNoRcv) &&
                (_rep_count < MAX_REP_COUNT)
            )
            {
                RUN_EVENT(12, source_address, priority, tsdu);
            }
            else if(
                (source_address == _connection_address) &&
                (T_SEQ_NO_OF_PDU(tsdu[0]) == _seqNoRcv) &&
                (_rep_count >= MAX_REP_COUNT)
            )
            {
                RUN_EVENT(13, source_address, priority, tsdu);
            }
            else if(source_address != _connection_address)
            {
                RUN_EVENT(14, source_address, priority, tsdu);
            }
            break;
        }
    }
}

/**
 * \fn T_Data_Individual.con(ack_request, hop_count_type, octet_count, priority, TSAP, tsdu,
 * t_status) \param ack_request: Data Link Layer Acknowledge requested or don’t care \param
 * hop_count_type: hop count 7 or Network Layer Parameter \param octet_count: length information as
 * described in Data Link Layer \param priority: highest, urgent, normal or low priority \param
 * TSAP: identifier of the service access point (may be direct the Individual Address of the remote
 * partner) \param tsdu: this is the user data that has been transferred by Transport Layer \param
 * t_status: ok: T_Data_Individual sent successfully with N_Data_Individual service not_ok:
 * transmission of the associated N_Data_Individual request frame did not succeed
 */
void T_Data_Individual__con(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                            Priority priority, uint8_t TSAP, uint8_t *tsdu, T_Status t_status)
{
    switch(pdu_type(tsdu[0])) {
        case DataInduvidual:
        {
            // TODO: application_data_individual_confirm(ack, hopType, priority, destination, tpdu.apdu(), status);
            break;
        }
        case DataConnected:
        {
            RUN_EVENT(22, TSAP, priority, tsdu);
            break;
        }
        case Connect:
        {
            if(t_status)
            {
#if defined(STYLE_3)
                int state = _current_state;
                RUN_EVENT(19, TSAP, priority, tsdu);
                if(state == CONNECTING)
                {
                    _current_state = OPEN_IDLE;
                }
#else
                RUN_EVENT(19, TSAP, priority, tsdu);
#endif
            }
            else
            {
                RUN_EVENT(20, TSAP, priority, tsdu);
            }
            break;
        }
        case Disconnect:
        {
            RUN_EVENT(21, TSAP, priority, tsdu);
            break;
        }
        case Ack:
        {
            RUN_EVENT(23, TSAP, priority, tsdu);
            break;
        }
        case Nack:
        {
            RUN_EVENT(24, TSAP, priority, tsdu);
            break;
        }
    }
}


/**
 * \fn T_Connect.req (destination_address, priority)
 * \param destination_address: Indivudual Address of the device to which the transport connection
 * shall be established. \param priority: highest, urgent, normal or low priority
 */
void T_Connect__req(KnxAddress destination_address, Priority priority)
{
#if defined(STYLE_3)
                int state = _current_state;
                RUN_EVENT(25, destination_address, priority, 0);
                if(state == CLOSED)
                {
                    _current_state = CONNECTING;
                }
#else
                RUN_EVENT(25, destination_address, priority, 0);
#endif
}


/**
 * \fn T_Disconnect.req(priority, TSAP)
 * \param priority highest, urgent, normal or low priority
 * \param TSAP identifier of the service access point to which the connection shall be released
 */
void T_Disconnect__req(Priority priority, uint8_t TSAP)
{
    RUN_EVENT(26, TSAP, priority, 0);
}



/**
 * \fn T_Data_Connected.req (octet_count, priority, TSAP, tsdu)
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param TSAP: identifier of the service access point to which the frame shall be sent
 * \param tsdu: this is the user data to be transferred by Transport Layer
 */
void T_Data_Connected__req(uint8_t octet_count, Priority priority, uint8_t TSAP, uint8_t *tsdu)
{
    RUN_EVENT(15, 0, priority, tsdu);
}



/**
 * \fn T_Data_Broadcast.req(ack_request, hop_count_type, octet_count, priority, tsdu)
 * \param ack_request: Data Link Layer Acknowledge requested or don’t care
 * \param hop_count_type: hop count 7 or Network Layer Parameter
 * \param octet_count: length information as described in Data Link Layer
 * \param priority: highest, urgent, normal or low priority
 * \param tsdu: This shall be the user data to be transferred by Transport Layer
 */
void T_Data_Broadcast__req(uint8_t ack_request, uint8_t hop_count_type, uint8_t octet_count,
                           Priority priority, uint8_t *tsdu)
{
#ifdef DEBUG_LAYER_TRANSPORT
    console_print_string("T_Data_Broadcast__req(");
    console_print_int(ack_request);
    console_print_char(',');
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_int(octet_count);
    console_print_char(',');
    print_priority(priority);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif
    N_Data_Broadcast__req(ack_request, hop_count_type, octet_count, priority, tsdu);
}

/**
 * \fn T_Data_Broadcast.ind(hop_count_type, octet_count, priority, source_address, tsdu)
 * \param hop_count_type hop count equals 7 or not
 * \param octet_count length information as described in Data Link Layer
 * \param priority highest, urgent, normal or low priority
 * \param source_address Individual Address of the device that requested the T_Data_Broadcast
 * service \param tsdu This shall be the user data that has been transferred by Transport Layer.
 */
void T_Data_Broadcast__ind(uint8_t hop_count_type, uint8_t octet_count, Priority priority,
                           KnxAddress source_address, uint8_t *tsdu)
{
#ifdef DEBUG_LAYER_TRANSPORT
    console_print_string("T_Data_Broadcast__ind(");
    console_print_int(hop_count_type);
    console_print_char(',');
    console_print_int(octet_count);
    console_print_char(',');
    print_priority(priority);
    console_print_char(',');
    print_source_address(source_address);
    console_print_string(", [");
    for (int i = 0; i < octet_count - 1; i++)
    {
        console_print_hex(tsdu[i]);
        console_print_char(' ');
    }
    console_print_hex(tsdu[octet_count - 1]);
    console_print_string("])\r\n");
#endif

    priority = SystemPriority;
    uint16_t pdu = tsdu[0] << 8 | tsdu[1];

    switch (pdu)
    {
    case A_IndividualAddress_Read:
        if (device_prog_mode())
        {
            A_IndividualAddress_Read__ind(priority, hop_count_type);
        }
        break;

    case A_IndividualAddress_Write:
        if (device_prog_mode())
        {
            console_print_string("A_IndividualAddress_Write TOCHECK\r\n");
            uint16_t addr = (tsdu[2] << 8) | (tsdu[3]);
            A_IndividualAddress_Write__ind(priority, hop_count_type, addr);
        }
        break;
    case A_DomainAddress_Read: /* 0x3E1 */
        if (device_prog_mode())
        {
            console_print_string("A_DomainAddress_Read TODO\r\n");
        }
        break;
    default:
        console_print_string("T_Data_Broadcast__ind not managed ");
        console_print_hex(pdu);
        console_print_string("\r\n");
        break;
    }
}
