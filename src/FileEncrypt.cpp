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

    wxMessageBox(errorMessage.IsEmpty()
                 ? wxString("Unknown OpenSSL error occurred.")
                 : errorMessage,
                 "SSL Error", wxOK | wxICON_ERROR);
}

int Encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext)
{
    EVP_CIPHER_CTX* ctx;

    int len;

    int ciphertext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        HandleOpenSSLErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        HandleOpenSSLErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        HandleOpenSSLErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        HandleOpenSSLErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


int Decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext)
{
    EVP_CIPHER_CTX* ctx;

    int len;
    int plaintext_len;

    if (!(ctx = EVP_CIPHER_CTX_new()))
        HandleOpenSSLErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        HandleOpenSSLErrors();

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        HandleOpenSSLErrors();
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        HandleOpenSSLErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

bool EncryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile)
{
    if (infile.empty() || outfile.empty())
    {
        wxMessageBox("Input or output path is empty!", "File Error", wxOK | wxICON_ERROR);
        return false;
    }

    std::ifstream in(infile, std::ios::binary);
    if (!in)
    {
        wxMessageBox("Failed to open input file!", "File Error", wxOK | wxICON_ERROR);
        return false;
    }

    std::ofstream out(outfile, std::ios::binary);
    if (!out)
    {
        wxMessageBox("Failed to create output file!", "File Error", wxOK | wxICON_ERROR);
        return false;
    }

    std::vector<unsigned char> iv(IV_LENGTH);
    if (!RAND_bytes(iv.data(), IV_LENGTH))
    {
        wxMessageBox("IV generation failed!", "Encryption Error", wxOK | wxICON_ERROR);
        out.close();
        return false;
    }

    out.write(reinterpret_cast<char*>(iv.data()), IV_LENGTH);

    std::vector<unsigned char> in_buf(BUFFER_SIZE);
    std::vector<unsigned char> out_buf(BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH);

    while (in.read(reinterpret_cast<char*>(in_buf.data()), BUFFER_SIZE) || in.gcount() > 0)
    {
        int read_len = static_cast<int>(in.gcount());

        int encrypted_len = 0;
        try
        {
            encrypted_len = Encrypt(in_buf.data(), read_len, key, iv.data(), out_buf.data());
        }
        catch (...)
        {
            wxMessageBox("Encryption failed mid-stream!", "Encryption Error", wxOK | wxICON_ERROR);
            out.close();
            return false;
        }

        out.write(reinterpret_cast<char*>(out_buf.data()), encrypted_len);
    }

    out.close();
    in.close();

    return true;
}

bool DecryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile)
{
    std::ifstream in(infile, std::ios::binary);
    std::ofstream out(outfile, std::ios::binary);

    if (!in || !out)
    {
        std::cerr << "Error opening files!" << std::endl;
        return false;
    }

    // Read the IV from the beginning of the file
    std::vector<unsigned char> iv(IV_LENGTH);
    in.read(reinterpret_cast<char*>(iv.data()), IV_LENGTH);
    if (in.gcount() != IV_LENGTH)
    {
        std::cerr << "Failed to read IV from input file!" << std::endl;
        return false;
    }

    std::vector<unsigned char> in_buf(BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH);
    std::vector<unsigned char> out_buf(BUFFER_SIZE);

    while (in.read(reinterpret_cast<char*>(in_buf.data()), BUFFER_SIZE) || in.gcount() > 0)
    {
        int read_len = static_cast<int>(in.gcount());
        int decrypted_len = Decrypt(in_buf.data(), read_len, key, iv.data(), out_buf.data());

        out.write(reinterpret_cast<char*>(out_buf.data()), decrypted_len);
    }

    return true;
}

std::vector<unsigned char> MakeKeyFromPassword(const std::string& password)
{
    std::vector<unsigned char> key(KEY_LENGTH);

    if (!PKCS5_PBKDF2_HMAC(password.c_str(), -1, NULL, 0, 1, EVP_sha256(), KEY_LENGTH, key.data()))
    {
        std::cerr << "Error: PKCS5_PBKDF2_HMAC failed.\n";
        return {};
    }

    return key;
}

std::string DecryptFileToString(unsigned char* key, const std::string& infile)
{
    std::ifstream in(infile, std::ios::binary);
    if (!in)
    {
        std::cerr << "Error opening input file!" << std::endl;
        return {};
    }

    // Read the IV from the beginning of the file
    std::vector<unsigned char> iv(IV_LENGTH);
    in.read(reinterpret_cast<char*>(iv.data()), IV_LENGTH);
    if (in.gcount() != IV_LENGTH)
    {
        std::cerr << "Failed to read IV!" << std::endl;
        return {};
    }

    std::vector<unsigned char> in_buf(BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH);
    std::vector<unsigned char> out_buf(BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH);
    std::string decrypted;

    while (in.read(reinterpret_cast<char*>(in_buf.data()), BUFFER_SIZE) || in.gcount() > 0)
    {
        int read_len = static_cast<int>(in.gcount());
        int decrypted_len = Decrypt(in_buf.data(), read_len, key, iv.data(), out_buf.data());
        decrypted.append(reinterpret_cast<char*>(out_buf.data()), decrypted_len);
    }

    return decrypted;
}


bool EncryptMessageGCM(const std::string& plaintext, const std::vector<unsigned char>& key, std::ofstream& out)
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
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key.data(), iv.data())) goto cleanup;

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


bool DecryptAllMessagesGCM(const std::vector<unsigned char>& key, const std::string& infile, std::vector<std::string>& messages)
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
        if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key.data(), iv.data())) goto fail;

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
