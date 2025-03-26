#include "LoginController.h"
#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <string>

LoginController::LoginController(LoginForm* loginForm, UserManager* userManager)
    : m_loginForm(loginForm), m_userManager(userManager)
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
    // Enable the login button only if there is text in the username and password box.
    if (!m_loginForm->PasswordTextBox->GetValue().IsEmpty() && !m_loginForm->UsernameTextBox->GetValue().IsEmpty())
    {
        m_loginForm->LoginButton->Enable(true);
    }
    else
    {
        m_loginForm->LoginButton->Enable(false);
    }
}

void LoginController::TryLogin(wxCommandEvent& event)
{
    std::string LoginUsername = m_loginForm->UsernameTextBox->GetValue().ToStdString();
    std::string userPassword = m_userManager->getPassword(m_loginForm->UsernameTextBox->GetValue().ToStdString());
    std::string passwordGuess = m_loginForm->PasswordTextBox->GetValue().ToStdString();
    if (userPassword == passwordGuess)
    {
        m_loginForm->Destroy(); //Kill popup
    }
}

