#include "Protocol.h"
#include "cstring"

std::array<char, 40> PierProtocol::encode_header(PierHeader header)
{
    std::array<char, 40> out{0};
    memcpy(out.data(), &(header.type), sizeof(SendType));
    memcpy(out.data() + sizeof(SendType), &(header.sender_GUID), sizeof(GUID));
    memcpy(out.data() + sizeof(SendType) + sizeof(GUID), &(header.channel_GUID), sizeof(GUID));
    memcpy(out.data() + 2 * sizeof(GUID) + sizeof(SendType), &(header.size), sizeof(uint32_t));
    
    return out;
}

PierProtocol::PierHeader PierProtocol::decode_header(boost::asio::const_buffer header)
{
    char *buf = (char *)header.data();

    SendType type;
    GUID sender;
    GUID channel;
    uint32_t size;

    memcpy(buf, &type, sizeof(SendType));
    buf += sizeof(SendType);
    memcpy(buf, &sender, sizeof(GUID));
    buf += sizeof(GUID);
    memcpy(buf, &channel, sizeof(GUID));
    buf += sizeof(GUID);
    memcpy(buf, &size, sizeof(uint32_t));

    PierHeader out 
    {
        .type = type,
        .sender_GUID = sender,
        .channel_GUID = channel,
        .size = size
    };
    
    return out;
}
