#pragma once
#include <unordered_map>
#include "STDimport.h"

class UserManager
{
public:
    // Constructor declarations if needed.
    UserManager() = default;

    // Function declarations
    void addUser(const std::string& username, const std::string& password);
    std::string getPassword(const std::string& username) const;

private:
    // Stores username and password pairs.
    std::unordered_map<std::string, std::string> userCredentials;
};