#pragma once
#include "ThePierGUI.h"
#include <wx/wx.h>

class LoginController
{
public:
    LoginController(LoginForm* form);

private:
    LoginForm* m_form;
    void BindEvents();

    void OnLoginInput(wxCommandEvent& event);
    void TryLogin(wxCommandEvent& event);
};