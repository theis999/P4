#pragma once
#include "ThePierGUI.h"
#include <wx/msgdlg.h>
#include "STDimport.h"
#include "iMessage.h"
#include "LoginController.h"
#include "Channel.h"
#include "MainLoginInterface.h"
#include "MainReceiveMessageInterface.h"
#include "Storage.h"
#include <sstream>

using std::to_string;

class Main : public ThePier, public MainLoginInterface, public MainReceiveMessageInterface
{
	wxString window_title = "The Pier";

public:
	User currentUser;
	string currentPassword;

	std::vector<unsigned char> encryption_key;
	std::stringstream encryptPath;
	std::stringstream encryptPathData;

	Main();

	Storage& GetStorage();
	void OnChannelsBox(wxCommandEvent& event);

	void DisplayMsg(iMessage& m);

	void OnSendTextChange(wxCommandEvent& event);
	void OnSendTextEnter(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);

	void SendHandler(wxTextCtrl* sendtext);

	bool Login(User user, string password);

	void ReceiveHandler(Channel *ch, iMessage msg);

	void DoLogin();
	void ClickCreateNewUser(wxCommandEvent& event);
	void LoginButtonClick(wxCommandEvent& event);
	void UpdateLoginButtonLabel();
	bool PromptLogin(std::string& outUsername);

	//void RunTest(wxCommandEvent& event);

	void OnAppClose(wxCloseEvent& event);
};
