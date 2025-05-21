#pragma once
#include <memory>
#include "Channel.h"
#include "iMessage.h"
#include "Storage.h"
#include "PierListener.h"
#include "PierClient.h"
#include "MainReceiveMessageInterface.h"


#include <boost/asio.hpp>

namespace PierProtocol
{

enum SendType : uint64_t
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

    PierHeader(SendType type_, GUID sender_guid, GUID channel_guid, uint32_t size_);

    std::string to_string();
    static PierHeader from_string(std::string header);
};

std::string ip_str_from_bytes(const std::byte ip[4]);


class ProtocolHandler
{
    
private:
    PierListener lstn;
    MainReceiveMessageInterface* mn;
};

void SendMSG(Channel &ch, iMessage msg, User &sender, Storage &storage);
std::vector<iMessage> SendMSGRequest(Channel &ch, Member memb, iMessage::shash hash, User &sender, Storage& storage);
void SendMSGMulti(Channel &ch, Member memb, std::vector<iMessage> msgs, User &sender, Storage &storage);
void SendSyncProbe(Channel &ch, iMessage::shash hash, User &sender, Storage &storage);
void SendSyncStatus(Channel &ch, Member memb, uint8_t flag, User &sender, Storage& storage);
std::vector<iMessage::shash> SendSHASHRequest(Channel &ch, Member memb, uint32_t global_i, uint32_t n, User &sender, Storage &storage);
void SendSHASHMulti(Channel &ch, Member memb, std::vector<iMessage::shash> hashes, User &sender, Storage& storage);

}
