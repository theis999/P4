#include "LoginController.h"

LoginController::LoginController(LoginForm* form) : m_form(form)
{
    BindEvents();
    m_form->LoginButton->Enable(false);
}

void LoginController::BindEvents()
{
    //bind events from GUI to functions
    m_form->UsernameTextBox->Bind(wxEVT_TEXT, &LoginController::OnLoginInput, this);
    m_form->PasswordTextBox->Bind(wxEVT_TEXT, &LoginController::OnLoginInput, this);
    m_form->UsernameTextBox->Bind(wxEVT_BUTTON, &LoginController::TryLogin, this);
    m_form->PasswordTextBox->Bind(wxEVT_BUTTON, &LoginController::TryLogin, this);
    m_form->LoginButton->Bind(wxEVT_BUTTON, &LoginController::TryLogin, this);
}

void LoginController::OnLoginInput(wxCommandEvent& event)
{
    // Enable the login button only if there is text in the username and password box.
    if (!m_form->PasswordTextBox->GetValue().IsEmpty() && !m_form->UsernameTextBox->GetValue().IsEmpty())
    {
        m_form->LoginButton->Enable(true);
    }
    else
    {
        m_form->LoginButton->Enable(false);
    }
}

void LoginController::TryLogin(wxCommandEvent& event)
{


    //if (LoginSucces)
    m_form->Destroy(); //Kill popup
}
