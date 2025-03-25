#include "PierProto.h"
#include "Storage.h"

PierRequest::PierRequest(RequestType req_type, uint32_t _sender_GUID, uint32_t _channel_GUID) {
    PierRequest();
    type = req_type;
    sender_GUID = _sender_GUID;
    channel_GUID = _channel_GUID;
}

JoinRequest::JoinRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket) {
    PierRequest(RequestType::JOIN, _sender_GUID, _channel_GUID);
    ticket = _ticket;
}

InviteRequest::InviteRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> _ticket) {
    PierRequest(RequestType::INVITE, _sender_GUID, _channel_GUID);
    ticket = _ticket;
}

MessageRequest::MessageRequest(uint32_t _sender_GUID, uint32_t _channel_GUID, std::vector<std::byte> message) {
    PierRequest(RequestType::MESSAGE, _sender_GUID, _channel_GUID);
    message_contents = message;
}
