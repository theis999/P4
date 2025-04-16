#pragma once

#include "Main.h"

bool Main::PromptLogin(std::string& outUsername)
{
	LoginForm* loginForm = new LoginForm(this);
	LoginController controller(loginForm, nullptr);

	if (loginForm->ShowModal() == wxID_OK)
	{
		outUsername = loginForm->UsernameTextBox->GetValue().ToStdString();
		delete loginForm;
		return true;
	}

	delete loginForm;
	return false;
}

void Main::DoLogin()
{
	std::string username;
	if (!PromptLogin(username))
	{
		currentUser = User::CreateUser("NoUser");
		CurrentUserLabel->SetLabelText("User: Not logged in");
		UpdateLoginButtonLabel();
		return;
	}

	auto maybeUser = User::LoadUserByName(username, "../users.txt");
	if (!maybeUser.has_value())
	{
		wxMessageBox("No user with that username exists.", "Login Failed", wxOK | wxICON_ERROR);
		currentUser = User::CreateUser("NoUser");
		CurrentUserLabel->SetLabelText("User: Not logged in");
		UpdateLoginButtonLabel();
		return;
	}

	currentUser = maybeUser.value();
	CurrentUserLabel->SetLabelText("User: " + currentUser.name);
	UpdateLoginButtonLabel();
}

void Main::ClickCreateNewUser(wxCommandEvent& event)
{
	wxTextEntryDialog dialog(this, "Enter a new username:", "Create New User");

	if (dialog.ShowModal() == wxID_OK)
	{
		std::string username = dialog.GetValue().ToStdString();

		if (username.empty())
		{
			wxMessageBox("Username cannot be empty.", "Error", wxOK | wxICON_ERROR);
			return;
		}

		auto maybeUser = User::LoadUserByName(username, "../users.txt");
		if (maybeUser.has_value())
		{
			wxMessageBox("Username already exists. Choose a different one.", "Error", wxOK | wxICON_ERROR);
			return;
		}

		User newUser = User::CreateUser(username);
		newUser.SaveToFile("../users.txt");

		wxMessageBox("User '" + dialog.GetValue() + "' created successfully.", "Success", wxOK | wxICON_INFORMATION);
	}
}

void Main::LoginButtonClick(wxCommandEvent& event)
{
	DoLogin();
}

void Main::UpdateLoginButtonLabel()
{
	if (currentUser.name == "NoUser")
	{
		LoginButton->SetLabel("Login");
	}
	else
	{
		LoginButton->SetLabel("Switch User");
	}
}
