#include "Main.h"
#include <wx/msgdlg.h>
#include "Channel.h"
#include "Storage.h"

void Main::RunTest(wxCommandEvent& event)
{
	// test code
	static Storage testStorage;
	
    testStorage.OpenStorage("../data.txt"); // expect the file to be located in the project root

	testStorage.GetCurrentChannel().sync();

}

