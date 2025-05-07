#pragma once
#include <memory>
#include "Channel.h"
#include "iMessage.h"
#include "Storage.h"
#include "PierListener.h"
#include "MainReceiveMessageInterface.h"


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

std::array<char, 40> encode_header(PierHeader header);
PierHeader decode_header(boost::asio::const_buffer header);

class ProtocolHandler
{
    
private:
    PierListener lstn;
    MainReceiveMessageInterface* mn;
};

void SendMSG(Channel ch, iMessage msg, User sender, Storage &storage);
void SendMSGRequest(Channel ch, Member memb, iMessage::shash hash, User sender);
void SendMSGMulti(Channel ch, Member memb, std::vector<iMessage> msgs, User sender);
void SendSyncProbe(Channel ch, iMessage::shash hash, User sender);
void SendSyncStatus(Channel ch, Member memb, uint8_t flag, User sender);
void SendSHASHRequest(Channel ch, Member memb, uint32_t amount, User sender);
void SendSHASHMulti(Channel ch, Member memb, std::vector<iMessage::shash> hashes, User sender);

}
