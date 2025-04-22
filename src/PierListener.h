#pragma once
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;
using boost::asio::io_context;

class tcp_connection : std::enable_shared_from_this<tcp_connection>
{

public:
	typedef std::shared_ptr<tcp_connection> ptr;

	// Create a smart-pointer to a new connection object.
	static ptr create(io_context& io) { return ptr(new tcp_connection(io)); };
	
	// Get the socket the connection is on.
	tcp::socket& get_socket() { return sock; };

	// Start receiving data over the connection.
	void start_receive();

	// Write data over the connection. The const_buffer must be valid while the write completes asynchronously.
	void start_write(boost::asio::const_buffer data);

private:
	tcp_connection(io_context& io) : io_(io), sock(io) {};

	// Handler function called after first read operation.
	void handle_first_read(const boost::system::error_code& err, size_t bytes_read);

	// Handler function called after subsequent reads.
	void handle_read(const boost::system::error_code& err, size_t bytes_read);
	
	// Handler function called after write.
	void handle_write(const boost::system::error_code& err, size_t bytes_sent);
	
	void read_msg_handler(boost::asio::streambuf& buf, const boost::system::error_code& err, size_t bytes_read);

	boost::asio::streambuf read_buf;
	tcp::socket sock;
	io_context& io_;
	std::array<char, 1024> recvbuf {0};

};

class PierListener
{
public:
	PierListener(io_context& io);
	static constexpr int default_listening_port = 10000;

private:
	// Start accepting connections.
	void start_accept();

	// Handler function called after accept.
	void handle_accept(tcp_connection::ptr new_conn, const boost::system::error_code& err);

	io_context& io_;
	tcp::acceptor acceptor;
};

