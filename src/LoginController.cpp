#include "LoginController.h"
#include <nlohmann/json.hpp>
#include <argon2.h>
#include <iostream>
#include <fstream>
#include <string>

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

using json = nlohmann::json;

void LoginController::TryLogin(wxCommandEvent& event)
{
	std::string password = "my_secure_password";
	std::string salt = "somesalt"; // use a random salt in real applications

	// Argon2 parameters
	uint32_t t_cost = 2;             // Number of iterations
	uint32_t m_cost = (1 << 16);     // 64 MiB memory usage
	uint32_t parallelism = 1;        // Number of threads
	uint32_t hash_len = 32;          // Length of the hash
	char hash[128];                  // Buffer to store the encoded hash

	// Create Argon2id hash
	int result = argon2id_hash_encoded(
		t_cost, m_cost, parallelism,
		password.c_str(), password.size(),
		salt.c_str(), salt.size(),
		hash_len,
		hash, sizeof(hash)
	);

	// Create JSON object
	json j;
	j["original"] = password;
	j["argon2_hash"] = std::string(hash);

	// Write to file
	std::ofstream out("hash_output.json");
	out << j.dump(4);
	out.close();

    m_form->Destroy(); //Kill popup
}
