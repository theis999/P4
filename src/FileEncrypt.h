#pragma once
#include <string>
#include <vector>

void HandleOpenSSLErrors(void);
/*Encrypt plaintext using AES256.*/
int Encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int Decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);
bool EncryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile);
bool DecryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile);
std::vector<unsigned char> MakeKeyFromPassword(const std::string& password);