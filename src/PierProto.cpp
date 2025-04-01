#include "PierProto.h"
#include "Storage.h"

PierRequest::PierRequest(RequestType req_type, uint32_t _sender_GUID, uint32_t _channel_GUID) {
    PierRequest();
    type = req_type;
    sender_GUID = _sender_GUID;
    channel_GUID = _channel_GUID;
}

JoinNetTransfer::JoinNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket) {
    PierRequest(RequestType::JOIN, _sender_GUID, _channel_GUID);
    ticket = _ticket;
}

InviteNetTransfer::InviteNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket) {
    PierRequest(RequestType::INVITE, _sender_GUID, _channel_GUID);
    ticket = _ticket;
}

MessageNetTransfer::MessageNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> message) {
    PierRequest(RequestType::MESSAGE, _sender_GUID, _channel_GUID);
    message_contents = message;
}

SyncProbeNetTransfer::SyncProbeNetTransfer(uint32_t _sender_GUID, uint32_t _channel_GUID, std::array<std::byte, 32> _hash) {
    PierRequest(RequestType::SYNC_PROBE, _sender_GUID, _channel_GUID);
    hash = _hash;
}

