#include "PierProto.h"
#include "Storage.h"
#include "boost/asio.hpp"

PierRequest::PierRequest(RequestType req_type, GUID _sender_GUID, GUID _channel_GUID) {
    PierRequest();
    type = req_type;
    sender_GUID = _sender_GUID;
    channel_GUID = _channel_GUID;
}

JoinNetTransfer::JoinNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<std::byte> _ticket) {
    PierRequest(RequestType::JOIN, _sender_GUID, _channel_GUID);
    ticket = _ticket;
}

InviteNetTransfer::InviteNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<std::byte> _ticket) {
    PierRequest(RequestType::INVITE, _sender_GUID, _channel_GUID);
    ticket = _ticket;
}

MessageNetTransfer::MessageNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::vector<std::byte> message) {
    PierRequest(RequestType::MESSAGE, _sender_GUID, _channel_GUID);
    message_contents = message;
}

SyncProbeNetTransfer::SyncProbeNetTransfer(GUID _sender_GUID, GUID _channel_GUID, std::array<std::byte, 32> _hash) {
    PierRequest(RequestType::SYNC_PROBE, _sender_GUID, _channel_GUID);
    hash = _hash;
}

