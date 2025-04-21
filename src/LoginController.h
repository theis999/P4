#pragma once
#include "ThePierGUI.h"
#include "User.h"
#include "MainLoginInterface.h"

class LoginController
{
public:
	LoginController(LoginForm* loginForm, User* user, MainLoginInterface* main);

private:
	LoginForm* m_loginForm;
	User* m_user;
	MainLoginInterface* main;
	void BindEvents();
	void OnLoginInput(wxCommandEvent& event);
	void TryLogin(wxCommandEvent& event);
	void ClickCreateNewUser(wxCommandEvent& event);
};
