#include "PierListener.h"
#include "Protocol.h"
#include <boost/bind.hpp>
#include "Signing.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

bool PierListener::syncing = false;

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

	std::string header_string;
	PierProtocol::PierHeader header = PierProtocol::PierHeader::from_string(dynbuf);
	std::string received( recvbuf.data() + header.to_string().length(), bytes_read - header.to_string().length());
	Storage &storage = mn->GetStorage();
	
	switch (header.type)
	{
		case PierProtocol::MESSAGE:
		{
			// Check if channel exists & if sender is part of channel.
			
			try
			{
				Channel& chan = storage.GetChannel(header.channel_GUID);
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
				std::string signature = iMsgFields[4];
				std::string text = iMsgFields[5];
				// Handle Signature verification
				string pubkey = Signing::readFileToString(".\\key.public.pem");
				bool verifySignature = Signing::oneStepVerifyMessage(pubkey.c_str(), signature.c_str(), text.c_str());
				if (TRUE == verifySignature)
				{
					// Construct an iMessage.
					iMessage msg(timestamp, memb_id, text, hash, chainhash, signature);

					mn->ReceiveHandler(this->channel, msg);
				}
			}
			catch (const std::exception&)
			{
				sock.close();
				return;
			}
			break;
		}
		case PierProtocol::SendType::SYNC_PROBE:
		{
			try
			{
				Channel& chan = storage.GetChannel(header.channel_GUID);
				std::stringstream ss(dynbuf);
				std::string field;
				for (size_t i = 0; i < 5; i++)
				{
					std::getline(ss, field, ';'); // Set field to be the shash
				}

				uint32_t inc_shash = std::stoul(field);

				iMessage::shash latest_shash = chan.messages.back().hash;
				uint32_t loc_shash = *(reinterpret_cast<uint32_t*>(latest_shash.data()));

				PierProtocol::PierHeader send_header
				{
					.type = PierProtocol::SYNC_STATUS,
					.sender_GUID = mn->GetCurrentUser().unique_id,
					.channel_GUID = chan.global_id,
					.size = 0,
				};
				std::string send{};

				if (loc_shash == inc_shash)
					send.append(std::format("{};", 2 /* ALL IS WELL */));
				else if (PierListener::syncing)
					send.append(std::format("{}", 0 /* ALREADY SYNCING - DENIED */));
				else
				{
					send.append(std::format("{}", 1 /* ACCEPT SYNC */));
					PierListener::syncing = true;
				}
				
				send_header.size = send.length();

				std::string out = send_header.to_string() + send;

				async_write(sock, buffer(out), boost::asio::detached);
				
			}
			catch (const std::exception&)
			{
				sock.close();
				return;
			}
			break;
		}
		case PierProtocol::SendType::SHASH_REQUEST:
		{
			try
			{
				Channel& chan = storage.GetChannel(header.channel_GUID);
				std::string field{};
				std::stringstream ss(dynbuf);
				std::vector<std::string> fields_vec{};

				for (size_t i = 0; i < 4; i++) // Skip header
				{
					std::getline(ss, field, ';');
				}
				for (size_t i = 0; i < 2; i++)
				{
					std::getline(ss, field, ';');
					fields_vec.push_back(field);
				}

				int global_i, n;
				global_i = atoi(fields_vec[0].c_str());
				n		 = atoi(fields_vec[1].c_str());


				std::vector<iMessage::shash> clientHashes{};

				for (int i = chan.messages.size() - global_i - 1; i >= 0 && i >= chan.messages.size() - global_i - n; --i)
				{
					clientHashes.push_back(chan.messages[i].hash);
				}

				PierProtocol::PierHeader send_header =
				{
					.type = PierProtocol::SHASH_MULTI,
					.sender_GUID = mn->GetCurrentUser().unique_id,
					.channel_GUID = chan.global_id,
					.size = 0,
				};
				std::string send;

				for (auto& hash : clientHashes)
				{
					send.append(std::format("{};", *reinterpret_cast<uint32_t*>(hash.data())));
				}

				send_header.size = send.length();
				std::string out = send_header.to_string() + send;

				async_write(sock, buffer(out), boost::asio::detached);
				
			}
			catch (const std::exception&)
			{
				sock.close();
				return;
			}
		}
		case PierProtocol::MESSAGE_MULTI:
		{
			try
			{
				Channel& chan = storage.GetChannel(header.channel_GUID);

				std::string field;
				std::vector<iMessage> msgs;
				std::stringstream ss(dynbuf);

				for (size_t i = 0; i < 4; i++) // Skip header
				{
					std::getline(ss, field, ';');
				}

				while (std::getline(ss, field, ';'))
				{
					time_t timestamp = stoi(field);

					std::getline(ss, field, ';');
					int memb_id = stoi(field); // Should be a GUID?

					std::getline(ss, field, ';');
					uint32_t h = stoul(field);
					iMessage::shash hash = *(reinterpret_cast<iMessage::shash*>(&h));

					std::getline(ss, field, ';');
					h = stoul(field);
					iMessage::shash chainhash = *(reinterpret_cast<iMessage::shash*>(&h));

					std::getline(ss, field, ';');
					std::string text = field;

					// Construct an iMessage.
					msgs.emplace_back(timestamp, memb_id, text, hash, chainhash);
				}

				for (auto& msg : msgs)
				{
					mn->ReceiveHandler(chan, msg);
				}
			}
			catch (const std::exception&)
			{
				sock.close();
			}

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

		//mn->ReceiveHandler(ch, msg);

	}

}

tcp_connection::tcp_connection(boost::asio::io_context& io, MainReceiveMessageInterface * _mn) : io_(io), sock(io)
{
	this->mn = _mn;
}



PierListener::PierListener(boost::asio::io_context& io, MainReceiveMessageInterface * _mn) : io_(io), acceptor(io, tcp::endpoint(tcp::v4(), default_listening_port)), wg(io.get_executor())
{
	syncing = false;
	io_thread = std::thread([=]{ io_.run(); });
	// Start accepting connections.
	this->mn = _mn;
	start_accept();
}

PierListener::~PierListener()
{
	wg.reset();
	io_thread.join();
}

void PierListener::SetRunning(bool running)
{
	app_running = running;
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
	if (app_running)
	{
		start_accept();
	}
}

