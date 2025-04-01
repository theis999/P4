#pragma once

#include <vector>
#include "Channel.h"
#include "iMessage.h"
#include "Storage.h"    

typedef uint16_t Port_t;

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

class PierRequest {
    protected:
    RequestType type;
    uint32_t sender_GUID;
    uint32_t channel_GUID;
    public:
    PierRequest(){};
    PierRequest(RequestType req_type, uint32_t _sender_GUID, uint32_t _channel_GUID);
};

// Request for joining a channel with a ticket.
class JoinNetTransfer : PierRequest {
    std::vector<std::byte> ticket;
 public:   
    JoinNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket);
};

// Request for inviting a peer to a channel, supplying the necessary ticket.
class InviteNetTransfer : PierRequest {
    std::vector<std::byte> ticket;
 public:
    InviteNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket);
};

class MessageNetTransfer : PierRequest {
    std::vector<std::byte> message_contents;
 public:
    MessageNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> message);
};

class MessageRequestNetTransfer : PierRequest {
    std::array<std::byte, 32> hash;
 public:
    MessageRequestNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::array<std::byte, 32> _hash);
};

class MessageMultiNetTransfer : PierRequest {
    std::vector<iMessage> msgs;
public:
    MessageMultiNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<iMessage> _msgs);

};

class SyncProbeNetTransfer : PierRequest {
    std::array<std::byte, 32> hash;
 public:
    // Needs to know how sync works before implementing.
    SyncProbeNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::array<std::byte, 32> _hash);
};

class SyncStatusNetTransfer : PierRequest {
    uint8_t status;
 public:
    SyncStatusNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, uint8_t status);
};


// No idea if we need/want this.
class ChangeIPRequest : PierRequest {
    std::string new_ip;
 public:
    ChangeIPRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::string _new_ip);
};
