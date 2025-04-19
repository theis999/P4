#pragma once
#include <boost/asio.hpp>
#include <vector>

using boost::asio::ip::tcp;
using boost::asio::io_context;

class PierClient
{
public:
	PierClient(io_context& io, tcp::endpoint endpoint);

	// Write a buffer to the connected endpoint.
	void write(boost::asio::const_buffer data);

	// Close the client.
	void close();

	static void write_several_peers(std::vector<tcp::endpoint> endpoints, boost::asio::const_buffer data);
private:
	void do_connect(const tcp::endpoint endpoint);
	void do_write(boost::asio::const_buffer data);
	
	bool connected = false;
	tcp::socket sock;
	io_context& io_;
	std::array<char, 1024> recvbuf;
};
