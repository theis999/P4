#include "PierListener.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::io_context;

void tcp_connection::start_receive()
{
	sock.async_receive(buffer(recvbuf), std::bind(&tcp_connection::handle_first_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
}

void tcp_connection::start_write(const_buffer data)
{
	sock.async_send(data, std::bind(&tcp_connection::handle_write, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
}

// TODO: Get the read data out.
void tcp_connection::handle_first_read(const boost::system::error_code& err, size_t bytes_read)
{
	if (!err)
	{
		// Decode header. 
		// Keep receiving.
		sock.async_receive(buffer(recvbuf), std::bind(&tcp_connection::handle_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
	}
	else
	{
		// Should maybe just close socket here.
		sock.async_receive(buffer(recvbuf), std::bind(&tcp_connection::handle_first_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));	
	}
}

void tcp_connection::handle_read(const boost::system::error_code& err, size_t bytes_read)
{
	if (!err)
	{
		// Apend buffer to string or something like that.
	}

	// Keep receiving. 
	sock.async_receive(buffer(recvbuf), std::bind(&tcp_connection::handle_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
}

void tcp_connection::handle_write(const boost::system::error_code& err, size_t bytes_sent)
{
	// Probably not good error handling.
	if (err)
	{
		// Close socket if a write-error occurs.
		sock.close();
	}
}

PierListener::PierListener(io_context& io) : io_(io), acceptor(io, tcp::endpoint(tcp::v4(), default_listening_port))
{
	// Start accepting connections.
	start_accept();
}

void PierListener::start_accept()
{
	tcp_connection::ptr new_conn = tcp_connection::create(io_);
	acceptor.async_accept(new_conn->get_socket(), std::bind(&PierListener::handle_accept, this, new_conn, placeholders::error));
}

void PierListener::handle_accept(tcp_connection::ptr new_conn, const boost::system::error_code& err)
{
	if (!err)
	{
		new_conn->start_receive();
	}

	// Continue accepting.
	start_accept();
}
