/*
--------------------------------------------------------------------------------------------------------------
Functions:
string signMessage(const std::string& privateKey, const std::string& message);
bool oneStepVerifyMessage(const char* publickey, const char* signatureB64, const char* message);

signMessage returns the privatekey signature of the message hash in base-64 encoded format, which can then be
passed to other parts of the program or as a textfile to verify authenticity

oneStepVerifyMessage returns either true or false. If false, the signature is invalid or an error has occured.
If true, the signature and associated message are authentically signed by the owner of the privatekey

--------------------------------------------------------------------------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/buffer.h>
#include <windows.h>
#include "Signing.h"

std::string Signing::readFileToString(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Signing::writeStringToFile(const std::string& path, const std::string& content) {
    std::cout << "[writeStringToFile] Attempting to write to: " << path << "\n";

    std::ofstream out(path, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        DWORD errCode = GetLastError();
        std::cerr << "[writeStringToFile] Failed to open file: " << path << "\n";
        std::cerr << "Windows error code: " << errCode << "\n";
        return;
    }

    out << content;

    if (!out.good()) {
        std::cerr << "[writeStringToFile] Write failed.\n";
    } else {
        std::cout << "[writeStringToFile] Successfully wrote " << content.length() << " bytes to " << path << "\n";
    }

    out.flush();
    out.close();
}


/*
---------------------
    BASE64 CODING    
---------------------
*/
std::string Signing::base64Encode(const unsigned char* buffer, size_t length) {
    std::cout << "[base64Encode] Starting encode. Input length: " << length << "\n";

    BIO* bio = nullptr;
    BIO* b64 = nullptr;
    BUF_MEM* bufferPtr = nullptr;

    b64 = BIO_new(BIO_f_base64());
    if (!b64) {
        std::cerr << "[base64Encode] BIO_new(BIO_f_base64) failed.\n";
        return "";
    }

    bio = BIO_new(BIO_s_mem());
    if (!bio) {
        std::cerr << "[base64Encode] BIO_new(BIO_s_mem) failed.\n";
        BIO_free(b64);
        return "";
    }

    b64 = BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    if (BIO_write(b64, buffer, (int)length) <= 0) {
        std::cerr << "[base64Encode] BIO_write failed.\n";
        BIO_free_all(b64);
        return "";
    }

    if (BIO_flush(b64) != 1) {
        std::cerr << "[base64Encode] BIO_flush failed.\n";
        BIO_free_all(b64);
        return "";
    }

    BIO_get_mem_ptr(b64, &bufferPtr);
    if (!bufferPtr || !bufferPtr->data || bufferPtr->length == 0) {
        std::cerr << "[base64Encode] bufferPtr invalid.\n";
        BIO_free_all(b64);
        return "";
    }

    std::cout << "[base64Encode] bufferPtr->length = " << bufferPtr->length << "\n";
    std::string encoded(bufferPtr->data, bufferPtr->length);

    BIO_free_all(b64);

    std::cout << "[base64Encode] Returning encoded string.\n";
    return encoded;
}

bool Signing::base64Decode(const std::string& b64input, unsigned char** buffer, size_t* length) {
    BIO* bio, * b64;
    int decodeLen = (int)(b64input.length() * 3 / 4); // estimate
    *buffer = (unsigned char*)OPENSSL_malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';

    bio = BIO_new_mem_buf(b64input.c_str(), -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    *length = BIO_read(bio, *buffer, (int)b64input.length());
    BIO_free_all(bio);

    return *length > 0;
}


/*
---------------------
  Pub- Privkey load    
---------------------
*/


EVP_PKEY* Signing::createPublicRSA(const std::string& key) {
    BIO* keybio = BIO_new_mem_buf(key.c_str(), -1);
    if (!keybio) {
        std::cerr << "Failed to create key BIO\n";
        return nullptr;
    }

    EVP_PKEY* pubKey = PEM_read_bio_PUBKEY(keybio, nullptr, nullptr, nullptr);
    if (!pubKey) {
        std::cerr << "Failed to read public key\n";
        ERR_print_errors_fp(stderr);
    }

    BIO_free(keybio);
    return pubKey;
}


EVP_PKEY* Signing::createPrivateRSA(const std::string& key)
{
    BIO* keybio = BIO_new_mem_buf(key.c_str(), -1);
    if (!keybio)
    {
        std::cerr << "Failed to create key BIO\n";
        return nullptr;
    }

    EVP_PKEY* privKey = PEM_read_bio_PrivateKey(keybio, nullptr, nullptr, nullptr);
    if (!privKey)
    {
        std::cerr << "Failed to read private key\n";
        ERR_print_errors_fp(stderr);
    }

    BIO_free(keybio);
    return privKey;
}


/*
---------------------
    Signing logic    
---------------------
*/

bool Signing::RSASign(EVP_PKEY* pkey, const unsigned char* msg, size_t msgLen, unsigned char** encMsg, size_t* encMsgLen) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    if (!ctx || EVP_DigestSignInit(ctx, NULL, EVP_sha256(), NULL, pkey) != 1) {
        std::cerr << "EVP_DigestSignInit failed\n";
        ERR_print_errors_fp(stderr);
        EVP_MD_CTX_free(ctx);
        return false;
    }

    if (EVP_DigestSignUpdate(ctx, msg, msgLen) != 1) {
        std::cerr << "EVP_DigestSignUpdate failed\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    if (EVP_DigestSignFinal(ctx, NULL, encMsgLen) != 1) {
        std::cerr << "EVP_DigestSignFinal (len) failed\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    *encMsg = (unsigned char*)OPENSSL_malloc(*encMsgLen);
    if (EVP_DigestSignFinal(ctx, *encMsg, encMsgLen) != 1) {
        std::cerr << "EVP_DigestSignFinal (sig) failed\n";
        EVP_MD_CTX_free(ctx);
        OPENSSL_free(*encMsg);
        return false;
    }

    EVP_MD_CTX_free(ctx);
    return true;
}


std::string Signing::signMessage(const std::string& privateKey, const std::string& message)
{
    std::cout << "[signMessage] Loading private key...\n";
    EVP_PKEY* pkey = createPrivateRSA(privateKey);
    if (!pkey) return "";

    std::cout << "[signMessage] Signing message...\n";
    unsigned char* signature = nullptr;
    size_t sigLen = 0;

    if (!RSASign(pkey, reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), &signature, &sigLen))
    {
        std::cerr << "[signMessage] Signing failed.\n";
        EVP_PKEY_free(pkey);
        return "";
    }

    std::string encoded = base64Encode(signature, sigLen);

    OPENSSL_free(signature);
    EVP_PKEY_free(pkey);

    return encoded;
}



/*
---------------------
   Verify Signature   
---------------------
*/


bool Signing::verifySignature(EVP_PKEY* pkey, const unsigned char* signature, size_t sigLen, const char* msg, size_t msgLen, bool* isAuthentic) {
    *isAuthentic = false;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx || EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, pkey) != 1) {
        std::cerr << "EVP_DigestVerifyInit failed\n";
        ERR_print_errors_fp(stderr);
        EVP_MD_CTX_free(ctx);
        return false;
    }

    if (EVP_DigestVerifyUpdate(ctx, msg, msgLen) != 1) {
        std::cerr << "EVP_DigestVerifyUpdate failed\n";
        EVP_MD_CTX_free(ctx);
        return false;
    }

    int result = EVP_DigestVerifyFinal(ctx, signature, sigLen);
    *isAuthentic = (result == 1);

    EVP_MD_CTX_free(ctx);
    return (result == 1 || result == 0);
}



/*
------------------------------------
    SIGN/VERIFY -main- functions    
------------------------------------
*/
// Use signMessage for signing

bool Signing::oneStepVerifyMessage(const char* publickey, const char* signatureB64, const char* message) {
    if (!publickey || !signatureB64 || !message || 
        strlen(publickey) == 0 || strlen(signatureB64) == 0 || strlen(message) == 0) {
        std::cerr << "[oneStepVerifyMessage] Input is invalid.\n";
        return false;
    }

    EVP_PKEY* pkey = createPublicRSA(publickey);
    if (!pkey) return false;

    unsigned char* decodedSig = nullptr;
    size_t decodedSigLen = 0;

    if (!base64Decode(signatureB64, &decodedSig, &decodedSigLen)) {
        std::cerr << "Failed to decode Base64 signature.\n";
        EVP_PKEY_free(pkey);
        return false;
    }

    bool authentic = false;
    bool result = verifySignature(pkey, decodedSig, decodedSigLen, message, strlen(message), &authentic);

    OPENSSL_free(decodedSig);
    EVP_PKEY_free(pkey);

    return result && authentic;
}
