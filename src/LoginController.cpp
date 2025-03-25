#include "LoginController.h"
#include <fstream>
#include <string>

LoginController::LoginController(LoginForm* loginForm, User* user)
    : m_loginForm(loginForm), m_user(user)
{
    BindEvents();
    m_loginForm->LoginButton->Enable(false);
}

void LoginController::BindEvents()
{
    m_loginForm->UsernameTextBox->Bind(wxEVT_TEXT, &LoginController::OnLoginInput, this);
    m_loginForm->PasswordTextBox->Bind(wxEVT_TEXT, &LoginController::OnLoginInput, this);
    m_loginForm->UsernameTextBox->Bind(wxEVT_COMMAND_TEXT_ENTER, &LoginController::TryLogin, this);
    m_loginForm->PasswordTextBox->Bind(wxEVT_COMMAND_TEXT_ENTER, &LoginController::TryLogin, this);
    m_loginForm->LoginButton->Bind(wxEVT_BUTTON, &LoginController::TryLogin, this);
}

void LoginController::OnLoginInput(wxCommandEvent& event)
{
    if (!m_loginForm->UsernameTextBox->GetValue().IsEmpty() && !m_loginForm->PasswordTextBox->GetValue().IsEmpty())
    {
        m_loginForm->LoginButton->Enable();
    }
}

void LoginController::TryLogin(wxCommandEvent& event)
{
    if (m_loginForm->LoginButton->IsEnabled())
    {
        std::string username = m_loginForm->UsernameTextBox->GetValue().ToStdString();
        std::string password = m_loginForm->PasswordTextBox->GetValue().ToStdString();

        auto maybeUser = User::LoadUserByName(username, "../users.txt");

        if (password == "secretkey" && maybeUser.has_value())
        {
            m_loginForm->EndModal(wxID_OK);
        }
        else
        {
            m_loginForm->WrongMessageLabel->Show();
            m_loginForm->Layout();
        }
    }
}