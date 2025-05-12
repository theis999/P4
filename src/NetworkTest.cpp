#include "PierClient.h"
#include "PierListener.h"
#include "Protocol.h"
#include "Uuid.h"
#include "Tests.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::asio::io_context;

void RunNetworkTest(MainReceiveMessageInterface *mn)
{
	// Listen first
	io_context io;

	PierListener lstn(io, mn);

	// Write a message to a channel:
	std::time_t t;
	iMessage msg(time(&t), 1 + mn->GetStorage().GetCurrentChannel().channel_id * 6 /* iMessage class needs to have GUIDS and not member_id */, "NetworkTest!");

	PierProtocol::SendMSG(mn->GetStorage().GetCurrentChannel(), msg, mn->GetCurrentUser(), mn->GetStorage());

}