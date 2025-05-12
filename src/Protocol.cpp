#include "Protocol.h"
#include "Storage.h"
#include <cstring>
#include "PierClient.h"
#include <ctime>
#include "MainReceiveMessageInterface.h"
#include "Uuid.h"




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
    send.append(std::format("{};", msg.timestamp));
    // Append member id
    send.append(std::format("{};", msg.member_id)); // NEEDS TO BE A GUID
    // Append hash
    send.append(std::format("{};", *(reinterpret_cast<uint32_t*>(msg.hash.data()) )));
    // Append chainhash
    send.append(std::format("{};", *(reinterpret_cast<uint32_t*>(msg.chainHash.data()))));
    // Append text last.
    send.append(msg.text);

    /* THESE ARE THE OLD NON-PLAINTEXT VERSIONS OF THE APPENDS */
    /*
    send.append(reinterpret_cast<const char*>(&(msg.timestamp)), sizeof(iMessage::timestamp));
    
    send.append(reinterpret_cast<const char*>(&(msg.member_id)), sizeof(iMessage::member_id));
    
    send.append(reinterpret_cast<const char*>(&(msg.hash)), sizeof(iMessage::hash));
    
    send.append(reinterpret_cast<const char*>(&(msg.chainHash)), sizeof(iMessage::chainHash));
   
    */
    
    header.size = send.length();

    std::string out = header.to_string();
    out.append(send);

    for (auto& mem : ch.members)
    {
        auto& user = storage.users.at(mem.second.user_id);

        std::string ip_string = std::format("{}.{}.{}.{}", 
            static_cast<uint8_t>(user.IPv4[0]), 
            static_cast<uint8_t>(user.IPv4[1]), 
            static_cast<uint8_t>(user.IPv4[2]), 
            static_cast<uint8_t>(user.IPv4[3])
            );
        endpoints.emplace_back(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip_string), 10000));
    }

    PierClient::write_several_peers(endpoints, boost::asio::buffer(out));
    
  
}

std::string PierProtocol::PierHeader::to_string()
{
    std::string out{};

    out.append(std::format("{};", static_cast<uint64_t>(type)));
    out.append(GuidToString(sender_GUID));
    out.append(";");
    out.append(GuidToString(channel_GUID));
    out.append(";");
    out.append(std::format("{};", size));

    return out;
}

PierProtocol::PierHeader PierProtocol::PierHeader::from_string(std::string header)
{
    
    
    vector<std::string> header_fields;

    std::stringstream ss(header);
    std::string field;
    size_t i = 0;
    while (std::getline(ss, field, ';') && i<4)
    {
        header_fields.push_back(field);
        i++;
    }

    PierHeader out
    {
        .type           = static_cast<SendType>(atoi(header_fields[0].c_str())),
        .sender_GUID    = GuidFromString(header_fields[1]),
        .channel_GUID   = GuidFromString(header_fields[2]),
        .size           = static_cast<uint32_t>(atoi(header_fields[3].c_str())),
       
    };

    return out;
}
