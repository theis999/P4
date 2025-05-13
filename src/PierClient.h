#pragma once
#include <boost/asio.hpp>
#include <vector>

using boost::asio::ip::tcp;
using boost::asio::io_context;

class PierClient
{
public:

	enum class ClientFlags : uint8_t
	{
		NO_ANSWER_EXPECTED = 0b00000000,
		EXPECTING_SYNC_ANSWER = 0b00000001,
		EXPECTING_SHASH_ANSWER = 0b00000010
	};

	PierClient(io_context& io, tcp::endpoint endpoint);
	PierClient(io_context& io, tcp::endpoint endpoint, uint8_t flags);

	// Write a buffer to the connected endpoint.
	void write(boost::asio::const_buffer data);

	// Close the client.
	void close();

	static void write_several_peers(std::vector<tcp::endpoint> endpoints, boost::asio::const_buffer data, PierClient::ClientFlags flags);
	
private:
	void do_connect(const tcp::endpoint endpoint);
	void do_write(boost::asio::const_buffer data);
	void handle_read(const boost::system::error_code& err, size_t bytes_read);
	void handle_data_send(const boost::system::error_code err, size_t bytes_sent);

	bool connected = false;
	tcp::socket sock;
	io_context& io_;
	std::string dynbuf{};
	std::array<char, 1024> recvbuf;
	
	ClientFlags flags_{};
};
