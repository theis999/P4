#include "LoginController.h"
#include <fstream>
#include <string>
#include "Storage.h"

LoginController::LoginController(LoginForm* loginForm, User* user, MainLoginInterface* main)
	: m_loginForm(loginForm), m_user(user), main(main)
{
	BindEvents();
}

void LoginController::BindEvents()
{
	m_loginForm->UsernameTextBox->Bind(wxEVT_TEXT, &LoginController::OnLoginInput, this);
	m_loginForm->PasswordTextBox->Bind(wxEVT_TEXT, &LoginController::OnLoginInput, this);
	m_loginForm->UsernameTextBox->Bind(wxEVT_COMMAND_TEXT_ENTER, &LoginController::TryLogin, this);
	m_loginForm->PasswordTextBox->Bind(wxEVT_COMMAND_TEXT_ENTER, &LoginController::TryLogin, this);
	m_loginForm->LoginButton->Bind(wxEVT_BUTTON, &LoginController::TryLogin, this);
	m_loginForm->CreateNewUserButton->Bind(wxEVT_BUTTON, &LoginController::ClickCreateNewUser, this);
}

void LoginController::OnLoginInput(wxCommandEvent& event)
{
	auto name = m_loginForm->UsernameTextBox->GetValue();
	auto password = m_loginForm->PasswordTextBox->GetValue();
	m_loginForm->LoginButton->Disable();
	m_loginForm->CreateNewUserButton->Disable();
	if (name.IsEmpty() || password.IsEmpty()) return;
	wxYield();

	if (Storage::UserNameExists(name.ToStdString(), "../users.txt"))
		m_loginForm->LoginButton->Enable();
	else 
		m_loginForm->CreateNewUserButton->Enable();
}

void LoginController::TryLogin(wxCommandEvent& event)
{
	if (m_loginForm->LoginButton->IsEnabled())
	{
		string username = m_loginForm->UsernameTextBox->GetValue().ToStdString();
		string password = m_loginForm->PasswordTextBox->GetValue().ToStdString();

		auto maybeUser = User::LoadUserByName(username, "../users.txt");

		if (maybeUser.has_value() && password == "s")
		{
			m_loginForm->EndModal(wxID_OK);
			main->Login(maybeUser.value(), password);
		}
		else
		{
			m_loginForm->WrongMessageLabel->Show();
			m_loginForm->Layout();
		}
	}
}

void LoginController::ClickCreateNewUser(wxCommandEvent& event)
{
	m_loginForm->CreateNewUserButton->Disable();
	wxYield();
	string username = m_loginForm->UsernameTextBox->GetValue().ToStdString();
	string password = m_loginForm->PasswordTextBox->GetValue().ToStdString();
	User user = User::CreateUser(username);
	auto success = Storage::CreateUser(user, "../users.txt");
	if (success)
	{
		m_loginForm->EndModal(wxID_OK);
		main->Login(user, password);
	}
	else
	{
		m_loginForm->WrongMessageLabel->Show(); // create another message to show
		m_loginForm->Layout();
	}
}
