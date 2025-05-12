#include "PierListener.h"
#include "Protocol.h"
#include <boost/bind.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

void tcp_connection::start_receive()
{
	async_read(sock, dynamic_buffer(dynbuf), std::bind(&tcp_connection::handle_first_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
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
		std::string header_string;
		PierProtocol::PierHeader header = PierProtocol::PierHeader::from_string(dynbuf);
		


		std::string received( recvbuf.data() + header.to_string().length(), bytes_read - header.to_string().length());
		

		Storage storage = mn->GetStorage();

		switch (header.type)
		{
			case PierProtocol::MESSAGE:
			{
				bool channel_exists = false;
				// Check if channel exists & if sender is part of channel.
				for (auto& ch : storage.channels)
				{
					
					if (ch.global_id == header.channel_GUID) 
					{
						channel_exists = true;
						this->channel = &ch;
						break;
					}
				}	
				if (!channel_exists)
				{
					sock.close();
					return;
				}
				
				std::stringstream ss(dynbuf);
				std::vector<std::string> iMsgFields;
				std::string field{};

				// Skip header
				for (size_t i = 0; i < 4; i++)
				{
					std::getline(ss, field, ';');

				}

				while (std::getline(ss, field, ';'))
				{
					iMsgFields.push_back(field);
				}

				time_t timestamp = stoi(iMsgFields[0]);
				int memb_id = stoi(iMsgFields[1]); // Should be a GUID?
				uint32_t h = stoul(iMsgFields[2]);
				iMessage::shash hash = *(reinterpret_cast<iMessage::shash*>(&h));
				h = stoul(iMsgFields[3]);
				iMessage::shash chainhash = *(reinterpret_cast<iMessage::shash*>(&h));
				std::string text = iMsgFields[4];
				// Construct an iMessage.
				iMessage msg(timestamp, memb_id, text, hash, chainhash);
				
				mn->ReceiveHandler(shared_from_this()->channel, msg);
				
			}
			default:
				// Invalid type, close socket.
				sock.close();
				break;
		}

		// Decode header. 
		// Keep receiving.
		//sock.async_receive(buffer(recvbuf), std::bind(&tcp_connection::handle_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
	}
	else
	{
		// Should maybe just close socket here.
		async_read(sock, buffer(recvbuf), std::bind(&tcp_connection::handle_first_read, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
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

void tcp_connection::read_msg_handler(const boost::system::error_code& err, size_t bytes_read)
{
	if (!err) 
	{
		// Make a string from the streambuf to act as a const buffer.
		std::string read_string = std::string(recvbuf.data(), bytes_read);
		const_buffer reintrp_buf = buffer(read_string);
		
		// Make a char pointer to the contents of the buffer.
		const char* buf_ptr = reinterpret_cast<const char*>(reintrp_buf.data());

		// reinterpret_cast to time_t and increment the pointer by the size of time_t.
		time_t ts = *(reinterpret_cast<const time_t*>(buf_ptr));
		buf_ptr += sizeof(time_t);

		// reinterpret_cast to int and increment the pointer by the size of int.
		int memb_id = *(reinterpret_cast<const int*>(buf_ptr));
		buf_ptr += sizeof(int);

		// <============================> 
		iMessage::shash hash = *(reinterpret_cast<const iMessage::shash*>(buf_ptr));
		buf_ptr += sizeof(iMessage::shash);

		// <============================> 
		iMessage::shash chainhash = *(reinterpret_cast<const iMessage::shash*>(buf_ptr));
		buf_ptr += sizeof(iMessage::shash);
		
		// Make a new const_buffer from an offset equal to the buf_ptr.
		const_buffer text_buf = reintrp_buf + sizeof(time_t) + sizeof(int) + 2 * sizeof(iMessage::shash);

		// Make a text string from the new buffer.
		std::string text(static_cast<const char *>(text_buf.data()), text_buf.size());

		// Construct an iMessage.
		iMessage msg(ts, memb_id, text, hash, chainhash);

		mn->ReceiveHandler(this->channel, msg);

	}

}

tcp_connection::tcp_connection(boost::asio::io_context& io, MainReceiveMessageInterface * _mn) : io_(io), sock(io)
{
	this->mn = _mn;
}



PierListener::PierListener(boost::asio::io_context& io, MainReceiveMessageInterface * _mn) : io_(io), acceptor(io, tcp::endpoint(tcp::v4(), default_listening_port))
{
	// Start accepting connections.
	this->mn = _mn;
	start_accept();
}

void PierListener::start_accept()
{
	tcp_connection::ptr new_conn = tcp_connection::create(io_, mn);
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

