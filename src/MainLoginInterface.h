#pragma once
#include "User.h"
#include <string>

// Abstract class (i.e. an interface) for passing a login information between Main and LoginController, without passing all of Main to LoginController.
class MainLoginInterface
{
public:
	/// <summary>
	/// Tries to login by reading this users data file
	/// </summary>
	/// <remarks>Pure virtual function</remarks>
	/// <param name="user">The user you want to login into, the name of which will be used to find their data file</param>
	/// <param name="password">The password for the user, which will be used to unlock their data file, the password is stored for locking the file in the future</param>
	/// <returns>Whether it was possible to unlock the data file, if not then the password is assumed to be wrong</returns>
	virtual bool Login(User user, std::string password) = 0;
};
