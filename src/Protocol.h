#pragma once
#include <memory>
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


}
