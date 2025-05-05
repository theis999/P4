#include "PierClient.h"
#include "Protocol.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::io_context;

PierClient::PierClient(io_context& io, tcp::endpoint endpoint) : io_(io), sock(io)
{
	do_connect(endpoint);
}

void PierClient::write(const_buffer header, const_buffer data)
{
	// We sleep the thread and try again if not connected.
	if (!connected)
	{
		// Probably needs other retry-solution. This could pause the program for a pretty long time.
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		write(header, data);
		return;
	}
	do_write(header, data);

}

void PierClient::close()
{
	sock.close();
}

void PierClient::write_several_peers(std::vector<tcp::endpoint> endpoints, const_buffer header, const_buffer data)
{
	io_context io;
	// Work guard stops io.run() from returning when it has ran out of work.
	auto wg = make_work_guard(io);

	// Create a thread for io_context.
	std::thread io_thread( [&io](){io.run();} );
	
	std::vector<PierClient> clients{};
	for (auto& peer : endpoints)
	{
		clients.emplace_back(io, peer);
		clients.back().write(header, data);
	}
	// Allow io.run() to return.
	wg.reset();
	io_thread.join();

}

void PierClient::do_connect(const tcp::endpoint endpoint)
{
	sock.async_connect(endpoint,
		[this, endpoint](boost::system::error_code err)
		{
			// Try again if connect fails. Probably a bit scary to do this infinitely...
			if (err)
			{
				do_connect(endpoint);
			}
			else
			{
				connected = true;
			}
		}
	);
}

void PierClient::do_write(const_buffer header, const_buffer data)
{
	sock.async_write_some(header, 
		[this, data](boost::system::error_code err, size_t bytes_sent) 
		{
			if (err)
			{
				// Handle error. Maybe try send again.
			}
			else
			{
				if (bytes_sent == sizeof(PierProtocol::PierHeader))
				{
					sock.async_write_some(data, std::bind(&PierClient::handle_data_send, this, placeholders::error, placeholders::bytes_transferred));
				}
				else
				{
					sock.close();
				}

			}
		}
	);
}

void PierClient::handle_read(const boost::system::error_code& err, size_t bytes_read)
{
	// Switch-case with different options for returns from peer.
}

void PierClient::handle_data_send(const boost::system::error_code err, size_t bytes_sent)
{
	if (!err)
	{
		// Receive possible answer from server.

		/*
		if (expecting_answer)
		*/
		
		if (false) // TEMPORARY
		{
			async_read(sock, buffer(recvbuf), std::bind(&PierClient::handle_read, this, placeholders::error, placeholders::bytes_transferred));
		}
	}
}
