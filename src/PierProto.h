#pragma once

#include <vector>
#include "Channel.h"
#include "iMessage.h"
#include "Storage.h"

typedef uint16_t Port_t;

enum RequestType {
    JOIN = 0,
    INVITE,
    MESSAGE,
    SYNC,
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
class JoinRequest : PierRequest {
    std::vector<std::byte> ticket;
 public:   
    JoinRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket);
};

// Request for inviting a peer to a channel, supplying the necessary ticket.
class InviteRequest : PierRequest {
    std::vector<std::byte> ticket;
 public:
    InviteRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket);
};

class MessageRequest : PierRequest {
    std::vector<std::byte> message_contents;
 public:
    MessageRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> message);
};

class SyncRequest : PierRequest {
    uint32_t checksum;
 public:
    // Needs to know how sync works before implementing.
    SyncRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, uint32_t _checksum);
};

// No idea if we need/want this.
class ChangeIPRequest : PierRequest {
    std::string new_ip;
 public:
    ChangeIPRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::string _new_ip);
};
