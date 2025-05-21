#include "FileEncrypt.h"
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <fstream>
#include <iostream>
#include <wx/wx.h>
#include <cstring>

const int GCM_IV_LEN = 12;
const int GCM_TAG_LEN = 16;

const int IV_LENGTH = 16;
const int KEY_LENGTH =32;
const int BUFFER_SIZE =4096;

void HandleOpenSSLErrors(void)
{
    char errBuf[256];
    wxString errorMessage;

    while (unsigned long errCode = ERR_get_error())
    {
        ERR_error_string_n(errCode, errBuf, sizeof(errBuf));
        errorMessage += wxString::Format("OpenSSL error: %s\n", errBuf);
    }

    wxMessageBox(errorMessage.IsEmpty() ? wxString("Unknown OpenSSL error occurred.") : errorMessage, "SSL Error", wxOK | wxICON_ERROR);
}

std::vector<unsigned char> MakeKeyFromPassword(const std::string& password, const std::string& username)
{
    std::vector<unsigned char> encryption_key(KEY_LENGTH);
    std::string KeyIngest = username+password;

    if (!PKCS5_PBKDF2_HMAC(KeyIngest.c_str(), -1, NULL, 0, 600000, EVP_sha256(), KEY_LENGTH, encryption_key.data()))
    {
        std::cerr << "Error: PKCS5_PBKDF2_HMAC failed.\n";
        return {};
    }

    return encryption_key;
}

bool EncryptMessageGCM(const std::string& plaintext, const std::vector<unsigned char>& encryption_key, std::ofstream& out)
{
    if (!out.is_open() || plaintext.empty()) return false;

    const int GCM_IV_LEN = 12;
    const int GCM_TAG_LEN = 16;

    std::vector<unsigned char> iv(GCM_IV_LEN);
    if (!RAND_bytes(iv.data(), GCM_IV_LEN))
        return false;

    std::vector<unsigned char> ciphertext(plaintext.size());
    std::vector<unsigned char> tag(GCM_TAG_LEN);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    int len = 0;
    int ciphertext_len = 0;
    uint32_t clen = 0;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) goto cleanup;
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LEN, NULL)) goto cleanup;
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, encryption_key.data(), iv.data())) goto cleanup;

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.data()), static_cast<int>(plaintext.size()))) goto cleanup;
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) goto cleanup;
    ciphertext_len += len;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, GCM_TAG_LEN, tag.data())) goto cleanup;

    clen = static_cast<uint32_t>(ciphertext_len);
    out.write(reinterpret_cast<const char*>(iv.data()), GCM_IV_LEN);
    out.write(reinterpret_cast<const char*>(&clen), sizeof(clen));
    out.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertext_len);
    out.write(reinterpret_cast<const char*>(tag.data()), GCM_TAG_LEN);

    EVP_CIPHER_CTX_free(ctx);
    return true;

cleanup:
    EVP_CIPHER_CTX_free(ctx);
    return false;
}


bool DecryptAllMessagesGCM(const std::vector<unsigned char>& encryption_key, const std::string& infile, std::vector<std::string>& messages)
{
    std::ifstream in(infile, std::ios::binary);
    if (!in.is_open()) return false;

    while (true)
    {
        std::vector<unsigned char> iv(GCM_IV_LEN);
        if (!in.read(reinterpret_cast<char*>(iv.data()), GCM_IV_LEN)) break;

        uint32_t clen;
        if (!in.read(reinterpret_cast<char*>(&clen), sizeof(clen))) break;

        std::vector<unsigned char> ciphertext(clen);
        if (!in.read(reinterpret_cast<char*>(ciphertext.data()), clen)) break;

        std::vector<unsigned char> tag(GCM_TAG_LEN);
        if (!in.read(reinterpret_cast<char*>(tag.data()), GCM_TAG_LEN)) break;

        std::vector<unsigned char> plaintext(clen);

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return false;

        int len, plaintext_len;

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) goto fail;
        if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LEN, NULL)) goto fail;
        if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, encryption_key.data(), iv.data())) goto fail;

        if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), clen)) goto fail;
        plaintext_len = len;

        if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, GCM_TAG_LEN, tag.data())) goto fail;

        if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) goto fail;
        plaintext_len += len;

        messages.emplace_back(reinterpret_cast<char*>(plaintext.data()), plaintext_len);

        EVP_CIPHER_CTX_free(ctx);
        continue;

    fail:
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    return true;
}
