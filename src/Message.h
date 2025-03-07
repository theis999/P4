#pragma once
#include "wx/wx.h"
#include "time.h"

class Message {
	wxString MessageContent;
	wxString UserName;
	std::time_t TimeSinceEpoch;
// TODO: ADD HASH TO MESSAGE?
public:
	wxString OutputMessage; 

	Message(std::time_t timestamp, wxString username, wxString messagecontent);
	
	wxString FormatToPrint();

};
