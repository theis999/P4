#pragma once
#include "ThePierGUI.h"
#include <wx/wx.h>
#include "UserManager.h"

class LoginController
{
public:
    LoginController(LoginForm* loginForm, UserManager* _userManager);

private:
    LoginForm* m_loginForm;
    UserManager* m_userManager;
    void BindEvents();

    void OnLoginInput(wxCommandEvent& event);
    void TryLogin(wxCommandEvent& event);


};