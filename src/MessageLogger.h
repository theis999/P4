#pragma once
#include <string>
#include <wx/textctrl.h>

using std::string;

// Saves the entire contents of the chat display 
// to a text file (default file name is provided in the parameter).
void SaveChatWindowToTextFile(wxTextCtrl* chatCtrl, const string& filename = "messagedata.txt");

// Loads chat history from a text file into the chat display.
void LoadChatHistoryFromTextFile(wxTextCtrl* chatCtrl, const string& filename = "messagedata.txt");
