#pragma once
#include "ThePierGUI.h"
#include <wx/wx.h>
#include "Channel.h"
#include "User.h"

class LoginController
{
public:
    LoginController(LoginForm* loginForm, User* user);
    bool ShowAndHandleLogin(std::string& outDisplayName);

private:
    LoginForm* m_loginForm;
    User* m_user;
    void BindEvents();
    void OnLoginInput(wxCommandEvent& event);
    void TryLogin(wxCommandEvent& event);
};