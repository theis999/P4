#include "Protocol.h"
#include "Storage.h"
#include <cstring>
#include "PierClient.h"
#include <ctime>
#include "MainReceiveMessageInterface.h"


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

void PierProtocol::SendMSG(Channel ch, iMessage msg, User sender, Storage &storage)
{
    std::vector<boost::asio::ip::tcp::endpoint> endpoints;

    PierHeader header
    {
        .type = MESSAGE,
        .sender_GUID = sender.unique_id,
        .channel_GUID = ch.global_id,
        .size = 0
    };

    std::string send;

    // Append timestamp;
    send.append(reinterpret_cast<const char*>(&(msg.timestamp)), sizeof(iMessage::timestamp));
    
    // Append member id
    send.append(reinterpret_cast<const char*>(&(msg.member_id)), sizeof(iMessage::member_id));
    
    // Append hash
    send.append(reinterpret_cast<const char*>(&(msg.hash)), sizeof(iMessage::hash));
    
    // Append chainhash
    send.append(reinterpret_cast<const char*>(&(msg.chainHash), sizeof(iMessage::chainHash)));
   
    // Append text last.
    send.append(msg.text);

    header.size = send.length();
    std::array<char, 40> header_arr = encode_header(header);
    boost::asio::const_buffer header_buf = boost::asio::buffer(header_arr);

    for (auto& mem : ch.members)
    {
        auto& user = storage.users.at(mem.second.user_id);

        std::string ip_string = std::format("%d.%d.%d.%d", 
            static_cast<uint8_t>(user.IPv4[0]), 
            static_cast<uint8_t>(user.IPv4[1]), 
            static_cast<uint8_t>(user.IPv4[2]), 
            static_cast<uint8_t>(user.IPv4[3])
            );
        endpoints.emplace_back(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip_string), 10000));
    }

    PierClient::write_several_peers(endpoints, header_buf, boost::asio::buffer(send));
    
}
