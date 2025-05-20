#pragma once

#include <string>            // for std::string
#include <openssl/rsa.h>     // for RSA
#include <cstddef>           // for size_t

class Signing
{
protected:
	static std::string base64Encode(const unsigned char* buffer, size_t length);
    static EVP_PKEY* createPrivateRSA(const std::string& key);
    static bool base64Decode(const std::string& b64input, unsigned char** buffer, size_t* length);
    static EVP_PKEY* createPublicRSA(const std::string& key);
    static bool verifySignature(EVP_PKEY* pkey, const unsigned char* signature, size_t sigLen, const char* msg, size_t msgLen, bool* isAuthentic);
    static void writeStringToFile(const std::string& path, const std::string& content);
    static bool RSASign(EVP_PKEY* pkey, const unsigned char* msg, size_t msgLen, unsigned char** encMsg, size_t* encMsgLen);
    
public:
    static bool oneStepVerifyMessage(const char* publickey, const char* signatureB64, const char* message);
    static std::string signMessage(const std::string& privateKey, const std::string& message);
    static std::string readFileToString(const std::string& path);

};
