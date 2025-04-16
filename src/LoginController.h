#pragma once
#include "ThePierGUI.h"
#include "User.h"

class LoginController
{
public:
	LoginController(LoginForm* loginForm, User* user);

private:
	LoginForm* m_loginForm;
	User* m_user;
	void BindEvents();
	void OnLoginInput(wxCommandEvent& event);
	void TryLogin(wxCommandEvent& event);
};
