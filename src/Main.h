#pragma once
#include "ThePierGUI.h"

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::pair;
using std::to_string;

class Main : public ThePier
{
	wxString window_title = "ThePier";

public:
	Main();

	void OnChannelsBox(wxCommandEvent& event);

	void OnSendTextChange(wxCommandEvent& event);
	void OnSendTextEnter(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);
};

