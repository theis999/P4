#ifndef UDP_HOLEPUNCHER_HPP
#define UDP_HOLEPUNCHER_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>

using boost::asio::ip::udp;

class UdpHolepuncher {
public:
    UdpHolepuncher(boost::asio::io_context& io_context, unsigned short port)
        : socket_(io_context, udp::endpoint(udp::v4(), port)) {
        start_receive();
    }

    void send_message(const std::string& message, const std::string& peer_ip, unsigned short peer_port) {
        udp::endpoint peer_endpoint(boost::asio::ip::make_address(peer_ip), peer_port);
        socket_.send_to(boost::asio::buffer(message), peer_endpoint);
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    std::string message(recv_buffer_.data(), bytes_recvd);
                    std::cout << "[RECEIVED] " << message << " from " << remote_endpoint_.address().to_string() 
                              << ":" << remote_endpoint_.port() << std::endl;
                    start_receive();
                }
            });
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
};

#endif // UDP_HOLEPUNCHER_HPP
