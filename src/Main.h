#pragma once
#include "ThePierGUI.h"
#include <wx/msgdlg.h>
#include "STDimport.h"
#include "iMessage.h"
#include "LoginController.h"
#include "Channel.h"

using std::to_string;

class Main : public ThePier
{
	wxString window_title = "The Pier";

public:
	User currentUser;
	Main();

	void OnChannelsBox(wxCommandEvent& event);

	void DisplayMsg(iMessage& m);

	void OnSendTextChange(wxCommandEvent& event);
	void OnSendTextEnter(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);

	void SendHandler(wxTextCtrl* sendtext);

	void DoLogin();
	void ClickCreateNewUser(wxCommandEvent& event);
	void LoginButtonClick(wxCommandEvent& event);
	void UpdateLoginButtonLabel();
	bool PromptLogin(std::string& outUsername);

	void RunTest(wxCommandEvent& event);

};
