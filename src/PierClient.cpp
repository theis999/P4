#include "PierClient.h"
#include "Protocol.h"
#include <cstdlib>

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::io_context;

PierClient::PierClient(io_context& io, tcp::endpoint endpoint) : io_(io), sock(io)
{
	do_connect(endpoint);
}

PierClient::PierClient(io_context& io, tcp::endpoint endpoint, uint8_t flags) : io_(io), sock(io)
{
	flags_ = ClientFlags{flags};
	do_connect(endpoint);
}

void PierClient::write(const_buffer data)
{
	// We sleep the thread and try again if not connected.
	
	try
	{
		if (sock.remote_endpoint().address() == boost::asio::ip::make_address("0.0.0.0"))
			return;
	}
	catch (const std::exception&)
	{
		return;
	}
	

	if (!connected)
		std::this_thread::sleep_for(std::chrono::seconds(2));
	if (!connected)
		return;
		
	do_write(data);

}

void PierClient::close()
{
	sock.close();
}

void PierClient::write_several_peers(std::vector<tcp::endpoint> endpoints, const_buffer data, PierClient::ClientFlags flags)
{
	io_context io;
	// Work guard stops io.run() from returning when it has ran out of work.
	auto wg = make_work_guard(io);

	// Create a thread for io_context.
	std::thread io_thread( [&io](){io.run();} );

	std::vector<PierClient> clients{};
	for (auto& peer : endpoints)
	{
		clients.emplace_back(io, peer, std::to_underlying(flags));
		clients.back().write(data);
	}
	// Allow io.run() to return.
	wg.reset();
	io_thread.join();

}

iMessage::shash PierClient::get_received_shash()
{
	if (flags_ != ClientFlags::EXPECTING_SYNC_ANSWER)
		return iMessage::shash();

	uint32_t h = std::stoul(received_shash);
	iMessage::shash hash = *(reinterpret_cast<iMessage::shash*>(&h));

	return hash;
}

void PierClient::do_connect(const tcp::endpoint endpoint)
{
	sock.async_connect(endpoint,
		[this, endpoint](boost::system::error_code err)
		{
			// Try again if connect fails. Probably a bit scary to do this infinitely...
			if (err)
			{
				return;
			}
			else
			{
				connected = true;
			}
		}
	);
}

void PierClient::do_write(const_buffer data)
{

	async_write(sock, data, std::bind(&PierClient::handle_data_send, this, placeholders::error, placeholders::bytes_transferred));
}

void PierClient::handle_read(const boost::system::error_code& err, size_t bytes_read)
{
	// Switch-case with different options for returns from peer.

	switch (flags_)
	{
		case PierClient::ClientFlags::NO_ANSWER_EXPECTED: [[unlikely]] 
			break;
		case PierClient::ClientFlags::EXPECTING_SYNC_ANSWER: [[likely]]
			{
				std::stringstream ss(dynbuf);
				std::string field;
				for (size_t i = 0; i < 5; i++) // Skip header
				{
					std::getline(ss, field, ';');
				}
				recvflag = atoi(field.c_str());
			}
			break;
		case PierClient::ClientFlags::EXPECTING_SHASH_ANSWER:
			{
				std::stringstream ss(dynbuf);
				std::string field;
				for (size_t i = 0; i < 4; i++) 
				{
					std::getline(ss, field, ';');
				}

				recv_shashes.clear();

				while (std::getline(ss, field, ';'))
				{
					uint32_t h = stoul(field);
					recv_shashes.push_back(*(reinterpret_cast<iMessage::shash*>(&h)));
				}
			}
			break;
		case PierClient::ClientFlags::EXPECTING_MULTIMSG_ANSWER:
		{
			std::stringstream ss(dynbuf);
			std::string field;
			for (size_t i = 0; i < 4; i++)
			{
				std::getline(ss, field, ';');
			}

			recv_messages.clear();

			while (std::getline(ss, field)) //while (std::getline(ss, field, ';'))
			{
				recv_messages.push_back(iMessage::from_str(field));
			}
		}
		break;
		default:
			// Ideally we don't get here.
			break;
	}

	sock.shutdown(tcp::socket::shutdown_both);
}

void PierClient::handle_data_send(const boost::system::error_code err, size_t bytes_sent)
{
	if (std::to_underlying(flags_))
	{
		async_read(sock, dynamic_buffer(dynbuf), std::bind(&PierClient::handle_read, this, placeholders::error, placeholders::bytes_transferred));
	} 
	else
	{
		sock.shutdown(tcp::socket::shutdown_both);
	}
}
