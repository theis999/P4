#pragma once

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <string>
#include <stdexcept>
#include <vector>

class MessageEncryption
{
protected:
    static std::string base64_encode(const std::vector<unsigned char>& buffer);
    static std::vector<unsigned char> base64_decode(const std::string& encoded);
    static void derive_key_iv(const std::string& password, std::vector<unsigned char>& key, std::vector<unsigned char>& iv);

public:
    static std::string encrypt_string(const std::string& plaintext, const std::string& password);
    static std::string decrypt_string(const std::string& b64ciphertext, const std::string& password);
};
