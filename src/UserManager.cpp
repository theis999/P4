#include "UserManager.h"

void UserManager::addUser(const std::string& username, const std::string& password)
{
    userCredentials[username] = password;
}

std::string UserManager::getPassword(const std::string& username) const
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end())
    {
        return it->second;
    }
    return "";
}
