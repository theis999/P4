#pragma once
#include "ThePierGUI.h"
#include <wx/msgdlg.h>
#include "STDimport.h"
#include "iMessage.h"
#include "LoginController.h"
#include "UserManager.h"

using std::to_string;

class Main : public ThePier
{
	wxString window_title = "ThePier";

public:
	Main();
	LoginForm* m_loginForm;
	LoginController* m_loginController;
	UserManager m_userManager;

	void OnChannelsBox(wxCommandEvent& event);

	void DisplayMsg(iMessage& m);

	void OnSendTextChange(wxCommandEvent& event);
	void OnSendTextEnter(wxCommandEvent& event);
	void OnSend(wxCommandEvent& event);

	void SendHandler(wxTextCtrl* sendtext);
};

