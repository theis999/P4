#include "Protocol.h"
#include "Storage.h"
#include <cstring>
#include "PierClient.h"
#include <ctime>
#include "MainReceiveMessageInterface.h"
#include "Uuid.h"


std::string PierProtocol::ip_str_from_bytes(const std::byte ip[4])
{
    std::string ip_string = std::format("{}.{}.{}.{}",
        std::to_integer<uint8_t>(ip[0]),
        std::to_integer<uint8_t>(ip[1]),
        std::to_integer<uint8_t>(ip[2]),
        std::to_integer<uint8_t>(ip[3])
    );
    
    return ip_string;
}

void PierProtocol::SendMSG(Channel &ch, iMessage msg, User &sender, Storage &storage)
{
    std::vector<boost::asio::ip::tcp::endpoint> endpoints;

    PierHeader header(MESSAGE, sender.unique_id, ch.global_id, 0);

    std::string send = msg.to_sc_sep_str();
    header.size = send.length();

    std::string out = header.to_string();
    out.append(send+";;");

    for (auto& mem : ch.members)
    {
        User& user = storage.users.at(mem.second.user_id);
        /*
        if (user.unique_id == sender.unique_id)
            continue;
        */

        std::string ip_string = ip_str_from_bytes(user.IPv4);
        if (ip_string != "0.0.0.0")
        {
            endpoints.emplace_back(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip_string), 10000));
        }
    }
    
    boost::asio::const_buffer buf = boost::asio::buffer(out);
    //boost::asio::const_buffer buf = boost::asio::buffer(out);

    PierClient::write_several_peers(endpoints, buf, PierClient::ClientFlags::NO_ANSWER_EXPECTED);
    
  
}

void PierProtocol::SendMSGMulti(Channel& ch, Member memb, std::vector<iMessage> msgs, User& sender, Storage& storage)
{
    User& user = storage.users.at(memb.user_id);

    PierHeader header(MESSAGE_MULTI, sender.unique_id, ch.global_id, 0);
    std::string send;

    for (auto& msg : msgs)
    {
        send.append(msg.to_sc_sep_str());
    }
    
    header.size = send.length();
    std::string out = header.to_string();
    out.append(send);

    std::vector<boost::asio::ip::tcp::endpoint> endpoint{};
    endpoint.emplace_back(boost::asio::ip::make_address(ip_str_from_bytes(user.IPv4)), 10000);

    PierClient::write_several_peers(endpoint, boost::asio::buffer(out), PierClient::ClientFlags::NO_ANSWER_EXPECTED);

}

void PierProtocol::SendSyncProbe(Channel &ch, iMessage::shash hash, User &sender, Storage& storage)
{
   
    // Prepare sync data in string format here.
    std::string send;
    send.append(std::format("{};", *reinterpret_cast<uint32_t*>(hash.data())));

    PierHeader header(SYNC_PROBE, sender.unique_id, ch.global_id, 0);
    header.size = send.length();

    std::string out = header.to_string();
    out.append(send);

    boost::asio::io_context io;

    std::vector<PierClient> clients{};

    std::vector<Member> members;

    for (auto& mem : ch.members)
    {
        User& user = storage.users.at(mem.second.user_id);
        std::string ip_string = ip_str_from_bytes(user.IPv4);
        clients.emplace_back(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(ip_string), 10000), std::to_underlying(PierClient::ClientFlags::EXPECTING_SYNC_ANSWER));
        members.push_back(mem.second);
    }

    boost::asio::const_buffer outbuf = boost::asio::buffer(out);

    auto memb_it = members.begin();

    for (auto& c : clients)
    {
        memb_it++;
        c.write(outbuf);
        io.run_one(); // Send
        io.run_one(); // Receive
        
        if (c.recvflag == 1)
        {
            ch.sync(*memb_it, sender, storage);
        }
    }

    //PierClient::write_several_peers(endpoints, boost::asio::buffer(sync_data), PierClient::ClientFlags::EXPECTING_SYNC_ANSWER);

}

void PierProtocol::SendSyncStatus(Channel& ch, Member memb, uint8_t flag, User& sender, Storage& storage)
{
    std::string data_str = std::format("{}", flag);
    PierHeader header(SendType::SYNC_STATUS, sender.unique_id, ch.global_id, 0);
    header.size = data_str.length();
    std::string out = header.to_string();

    out.append(data_str);
    
    std::vector<tcp::endpoint> endpoint;
    User& user = storage.users.at(memb.user_id);
    endpoint.emplace_back(boost::asio::ip::make_address(ip_str_from_bytes(user.IPv4)), 10000);

    PierClient::write_several_peers(endpoint, boost::asio::buffer(out), PierClient::ClientFlags::NO_ANSWER_EXPECTED);
}

std::vector<iMessage::shash> PierProtocol::SendSHASHRequest(Channel& ch, Member memb, uint32_t global_i, uint32_t n, User& sender, Storage &storage)
{
    PierHeader header(SendType::SHASH_REQUEST, sender.unique_id, ch.global_id, 0);

    std::string send = std::format("{};{};", global_i, n);
    header.size = send.length();

    std::string out = header.to_string() + send;

    User& user = storage.users.at(memb.user_id);
    
    tcp::endpoint endpoint(boost::asio::ip::make_address(ip_str_from_bytes(user.IPv4)), 10000);

    boost::asio::io_context io;
    PierClient c(io, endpoint, std::to_underlying(PierClient::ClientFlags::EXPECTING_SHASH_ANSWER));
    c.write(boost::asio::buffer(out));
    io.run();

    return c.recv_shashes;
    
}

std::vector<iMessage> PierProtocol::SendMSGRequest(Channel& ch, Member memb, iMessage::shash hash, User& sender, Storage& storage)
{
    PierHeader header(SendType::MESSAGE_REQUEST, sender.unique_id, ch.global_id, 0);

    std::string send = std::format("{};", iMessage::hash_to_string2(hash));
    header.size = send.length();

    std::string out = header.to_string() + send;

    User& user = storage.users.at(memb.user_id);

    tcp::endpoint endpoint(boost::asio::ip::make_address(ip_str_from_bytes(user.IPv4)), 10000);

    boost::asio::io_context io;
    PierClient c(io, endpoint, std::to_underlying(PierClient::ClientFlags::EXPECTING_MULTIMSG_ANSWER));
    c.write(boost::asio::buffer(out));
    io.run();

    return c.recv_messages;

}


PierProtocol::PierHeader::PierHeader(SendType type_, GUID sender_guid, GUID channel_guid, uint32_t size_)
    : type(type_), sender_GUID(sender_guid), channel_GUID(channel_guid), size(size_)
{

}

std::string PierProtocol::PierHeader::to_string()
{
    std::string out{};

    out.append(std::format("{};", static_cast<uint64_t>(type)));
    out.append(std::format("{};", GuidToString(sender_GUID)));
    out.append(std::format("{};", GuidToString(channel_GUID)));
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

    PierHeader out(
        static_cast<SendType>(atoi(header_fields[0].c_str())),
        GuidFromString(header_fields[1]),
        GuidFromString(header_fields[2]),
        static_cast<uint32_t>(std::stoul(header_fields[3]))
    );
    
    return out;
}
