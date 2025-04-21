#include "Protocol.h"
#include "Storage.h"
#include "cstring"
#include "PierClient.h"

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

void PierProtocol::SendMSG(Channel ch, iMessage msg)
{
    std::vector<boost::asio::ip::tcp::endpoint> endpoints;

    PierHeader header
    {
        .type = MESSAGE,
        //.sender_GUID = user_guid
        .channel_GUID = ch.channel_id,
        .size = msg.text.length(),
    };

    std::array<char, 40> header_arr = encode_header(header);
    
    std::string send(header_arr.data());
    send.append(msg.text);
    //send.append(msg.timestamp);

    boost::asio::const_buffer header_buf = boost::asio::buffer(header_arr);

    for (auto& mem : ch.members)
    {
        auto& user = storage.users.at(mem.user_id);

        // TODO: store ipv4 as a string
        //endpoints.emplace_back(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address((char*)user.IPv4), 10000));
    }

    PierClient::write_several_peers(endpoints, header_buf);
    
}
