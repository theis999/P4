#pragma once

#include <vector>
#include "Channel.h"
#include "iMessage.h"
#include "Storage.h"
#include <guiddef.h>

//typedef uint16_t Port_t;

enum RequestType
{
    JOIN = 0,
    INVITE,
    MESSAGE,
    MESSAGE_REQUEST,
    MESSAGE_MULTI,
    SYNC_PROBE,
    SYNC_STATUS,
    SHASH_REQUEST,
    SHASH_MULTI,
    CHANGEIP,
    REQUESTTYPE_END
};

struct PierRequest {
    RequestType type;
    GUID sender_GUID;
    GUID channel_GUID;
    PierRequest(){};
    PierRequest(RequestType req_type, GUID _sender_GUID, GUID _channel_GUID);
};

// Request for joining a channel with a ticket.
struct JoinNetTransfer : PierRequest {
    std::vector<std::byte> ticket;  
    JoinNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<std::byte> _ticket);
};

// Request for inviting a peer to a channel, supplying the necessary ticket.
struct InviteNetTransfer : PierRequest {
    std::vector<std::byte> ticket;
    InviteNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<std::byte> _ticket);
};

struct MessageNetTransfer : PierRequest {
    std::vector<std::byte> message_contents;
    MessageNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<std::byte> message);
};

struct MessageRequestNetTransfer : PierRequest {
    std::array<std::byte, 32> hash;
    MessageRequestNetTransfer(GUID _sender_GUID, GUID _channel_GUID, iMessage::shash _hash);
};

struct MessageMultiNetTransfer : PierRequest {
    std::vector<iMessage> msgs;
    MessageMultiNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<iMessage> _msgs);

};

struct SyncProbeNetTransfer : PierRequest {
    iMessage::shash hash;
    // Needs to know how sync works before implementing.
    SyncProbeNetTransfer(GUID _sender_GUID, GUID _channel_GUID, iMessage::shash _hash);
};

struct SyncStatusNetTransfer : PierRequest {
    uint8_t status;
    SyncStatusNetTransfer(GUID _sender_GUID, GUID _channel_GUID, uint8_t status);
};


// No idea if we need/want this.
struct ChangeIPRequest : PierRequest {
    std::string new_ip;
    ChangeIPRequest(GUID _sender_GUID, GUID _channel_GUID, std::string _new_ip);
};
