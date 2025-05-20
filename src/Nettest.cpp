#include "PierClient.h"
#include "PierListener.h"
#include "Nettest.h"
#include <boost/asio.hpp>
#include "Protocol.h"

//std::vector<PierClient> client;

void Nettest::establishConn()
{
	string host = "127.0.0.1";
	int port = 10000;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
	boost::asio::io_context io;
	//client.emplace_back(io, endpoint);
	//	client[0].write(mbuffer);
	boost::asio::const_buffer mbuffer = boost::asio::buffer("message");

	auto client = PierClient(io, endpoint);
	client.write(mbuffer);
};


void Nettest::sendMessage(string message)
{
	establishConn();
	/*string testheader = "TESTING123 Mike check ... Something -- ";
	boost::asio::const_buffer hbuffer = boost::asio::buffer(testheader);
	boost::asio::const_buffer mbuffer = boost::asio::buffer(message);
	client[0].write(mbuffer);*/
};