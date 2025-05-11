#pragma once
#include <string>
#include <vector>
#include <sstream>

void HandleOpenSSLErrors(void);
/*Encrypt plaintext using AES256.*/
int Encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int Decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);
bool EncryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile);
bool DecryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile);
std::vector<unsigned char> MakeKeyFromPassword(const std::string& password);
std::string DecryptFileToString(unsigned char* key, const std::string& infile);

bool EncryptMessageGCM(const std::string& plaintext, const std::vector<unsigned char>& key, std::ofstream& out);
bool DecryptAllMessagesGCM(const std::vector<unsigned char>& key, const std::string& infile, std::vector<std::string>& messages);

