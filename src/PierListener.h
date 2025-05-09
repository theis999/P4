#pragma once
#include "MainReceiveMessageInterface.h"
#include <boost/asio.hpp>
#include <memory>
#include "Channel.h"

using boost::asio::ip::tcp;
using boost::asio::io_context;

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{

public:
	typedef std::shared_ptr<tcp_connection> ptr;

	// Create a smart-pointer to a new connection object.
	static ptr create(boost::asio::io_context& io, MainReceiveMessageInterface* mn) { return ptr(new tcp_connection(io, mn)); };
	
	// Get the socket the connection is on.
	tcp::socket& get_socket() { return sock; };

	// Start receiving data over the connection.
	void start_receive();

	// Write data over the connection. The const_buffer must be valid while the write completes asynchronously.
	void start_write(boost::asio::const_buffer data);

private:
	tcp_connection(boost::asio::io_context& io, MainReceiveMessageInterface* _mn);

	// Handler function called after first read operation.
	void handle_first_read(const boost::system::error_code& err, size_t bytes_read);

	// Handler function called after subsequent reads.
	void handle_read(const boost::system::error_code& err, size_t bytes_read);
	
	// Handler function called after write.
	void handle_write(const boost::system::error_code& err, size_t bytes_sent);
	
	void read_msg_handler(const boost::system::error_code& err, size_t bytes_read);
	
	Channel *channel;
	MainReceiveMessageInterface* mn = nullptr;
	tcp::socket sock;
	boost::asio::io_context& io_;
	std::array<char, 1024> recvbuf {0};

};

class PierListener
{
public:
	PierListener(boost::asio::io_context& io, MainReceiveMessageInterface *_mn);
	static constexpr int default_listening_port = 10000;

private:
	// Start accepting connections.
	void start_accept();

	// Handler function called after accept.
	void handle_accept(tcp_connection::ptr new_conn, const boost::system::error_code& err);

	MainReceiveMessageInterface* mn;
	boost::asio::io_context& io_;
	tcp::acceptor acceptor;
};

