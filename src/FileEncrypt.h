#pragma once
#include <string>
#include <vector>
#include <sstream>

void HandleOpenSSLErrors(void);
std::vector<unsigned char> MakeKeyFromPassword(const std::string& password, const std::string & username);

bool EncryptMessageGCM(const std::string& plaintext, const std::vector<unsigned char>& encryption_key, std::ofstream& out);
bool DecryptAllMessagesGCM(const std::vector<unsigned char>& encryption_key, const std::string& infile, std::vector<std::string>& messages);

