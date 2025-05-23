#include "Main.h"
#include "MainReceiveMessageInterface.h"
#include <wx/msgdlg.h>
#include "Channel.h"
#include "Storage.h"
#include "Tests.h"
#include <thread>

static Storage testStorage;
Storage peerTestStorage;

void Main::RunTest(wxCommandEvent& event)
{
	try
	{
		Member m;

		for (auto& memb : this->GetStorage().GetCurrentChannel().members)
		{
			if (memb.second.name == "kristian")
			{
				m = memb.second;
				break;
			}
		}

		this->GetStorage().GetCurrentChannel().sync(m, currentUser, this->GetStorage());
	}
	catch (const std::exception& e)
	{
		return;
		//throw "haha hvad okay";
	}
	
	
	//std::thread t([&]{RunNetworkTest(this);});
	//t.detach();

	// test code
	/*
	testStorage.OpenStorage("../data.txt"); // expect the file to be located in the project root
	peerTestStorage.OpenStorage("../data.txt");

	time_t timestamp;
	time(&timestamp);

	iMessage testMessage0 = iMessage(timestamp, 00, "Hello World");
	iMessage testMessage1 = iMessage(timestamp, 42, "Life the universe and everything");
	iMessage testMessage2 = iMessage(timestamp, 01, "Hello friends!");
	iMessage testMessage3 = iMessage(timestamp, 13, "Skillet - Awake & Alive");
	iMessage testMessage4 = iMessage(timestamp, 17, "I ran out of ideas");
	iMessage testMessage5 = iMessage(timestamp, 21, "Politics");

	testStorage.GetCurrentChannel().messages.push_back(testMessage0);
	testStorage.GetCurrentChannel().messages.push_back(testMessage1);
	testStorage.GetCurrentChannel().messages.push_back(testMessage2);
	//testStorage.GetCurrentChannel().messages.push_back(testMessage3);
	//testStorage.GetCurrentChannel().messages.push_back(testMessage4);
	//testStorage.GetCurrentChannel().messages.push_back(testMessage5);

	peerTestStorage.GetCurrentChannel().messages.push_back(testMessage0);
	peerTestStorage.GetCurrentChannel().messages.push_back(testMessage1);
	peerTestStorage.GetCurrentChannel().messages.push_back(testMessage2);
	peerTestStorage.GetCurrentChannel().messages.push_back(testMessage3);
	peerTestStorage.GetCurrentChannel().messages.push_back(testMessage4);
	peerTestStorage.GetCurrentChannel().messages.push_back(testMessage5);

	testStorage.GetCurrentChannel().sync();
	*/

}

