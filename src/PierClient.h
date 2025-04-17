#pragma once
#include <boost/asio.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::io_context;

class PierClient
{
public:
	PierClient(io_context& io, tcp::endpoint endpoint);

	void write(const_buffer data);
	void close();

private:
	void do_connect(const tcp::endpoint endpoint);
	void do_write(const_buffer data);
	
	bool connected = false;
	tcp::socket sock;
	io_context& io_;
	std::array<char, 1024> recvbuf;
};
