#pragma once
#include <memory>
#include "Channel.h"
#include "iMessage.h"
#include "Storage.h"

#include <boost/asio.hpp>

namespace PierProtocol
{

enum SendType
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
    SENDTYPE_END
};

struct PierHeader
{
    SendType type;
    GUID sender_GUID;
    GUID channel_GUID;
    uint32_t size;
};

static Storage storage;

std::array<char, 40> encode_header(PierHeader header);
PierHeader decode_header(boost::asio::const_buffer header);

void SendMSG(Channel ch, iMessage msg);
void SendMSGRequest(Channel ch, Member memb, iMessage::shash hash);
void SendMSGMulti(Channel ch, Member memb, std::vector<iMessage> msgs);
void SendSyncProbe(Channel ch, iMessage::shash hash);
void SendSyncStatus(Channel ch, Member memb, uint8_t flag);
void SendSHASHRequest(Channel ch, Member memb, uint32_t amount);
void SendSHASHMulti(Channel ch, Member memb, std::vector<iMessage::shash> hashes);

}
