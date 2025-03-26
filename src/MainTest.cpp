#include "Main.h"
#include <wx/msgdlg.h>
#include "Channel.h"
#include "Storage.h"

void Main::RunTest(wxCommandEvent& event)
{
	// test code
	static Storage testStorage;
	
    testStorage.OpenStorage("../data.txt"); // expect the file to be located in the project root
	
	time_t timestamp;
	time(&timestamp);

	iMessage testMessage = iMessage(timestamp,42,"Life the universe and everything");
	iMessage testMessage2 = iMessage(timestamp, 42, "Hello friends!");
	iMessage testMessage3 = iMessage(timestamp, 13, "Skillet - Awake & Alive");
	iMessage testMessage4 = iMessage(timestamp, 17, "I ran out of ideas");
	iMessage testMessage5 = iMessage(timestamp, 13, "Skillet - Awake & Alive");
	//iMessage testMessage6 = iMessage(timestamp, 21, "Politics");
	testStorage.GetCurrentChannel().messages.push_back(testMessage);
	testStorage.GetCurrentChannel().messages.push_back(testMessage2);
	testStorage.GetCurrentChannel().messages.push_back(testMessage3);
	testStorage.GetCurrentChannel().messages.push_back(testMessage4);
	testStorage.GetCurrentChannel().messages.push_back(testMessage5);
	//testStorage.GetCurrentChannel().messages.push_back(testMessage6);

	testStorage.GetCurrentChannel().sync();

}

