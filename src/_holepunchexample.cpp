#include "UdpHolepuncher.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>

int main() {
    boost::asio::io_context io_context;

    // Create holepuncher instance with iocontext and port 
    UdpHolepuncher holepuncher(io_context, 50000);

    // New thread for holepuncher
    std::thread io_thread([&io_context] { io_context.run(); });

    std::string message;
    std::string peer_ip = "192.168.1.101";  
    unsigned short peer_port = 50000;       

    std::cout << "Start sending messages (press enter without text to exit)\n";

    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        if (message.empty()) break;

        holepuncher.send_message(message, peer_ip, peer_port);
    }

    io_context.stop();
    io_thread.join();

    return 0;
}
