#include "FileEncrypt.h"
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <fstream>
#include <iostream>
#include <wx/wx.h>

const int IV_LENGTH = 16;
const int KEY_LENGTH =32;
const int BUFFER_SIZE =4096;

void HandleOpenSSLErrors(void)
{
    char errBuf[256];
    wxString errorMessage;

    // Collect all OpenSSL errors
    while (unsigned long errCode = ERR_get_error())
    {
        ERR_error_string_n(errCode, errBuf, sizeof(errBuf));
        errorMessage += wxString::Format("OpenSSL error: %s\n", errBuf);
    }

    // Show error in a message box
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

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        HandleOpenSSLErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        HandleOpenSSLErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        HandleOpenSSLErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
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

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        HandleOpenSSLErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        HandleOpenSSLErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        HandleOpenSSLErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        HandleOpenSSLErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

bool EncryptFiles(unsigned char* key, const std::string& infile, const std::string& outfile)
{
    std::ifstream in(infile, std::ios::binary);
    std::ofstream out(outfile, std::ios::binary);

    if (!in || !out)
    {
        std::cerr << "Error opening files!" << std::endl;
        return false;
    }

    std::vector<unsigned char> iv(IV_LENGTH);
    if (!RAND_bytes(iv.data(), IV_LENGTH))
    {
        std::cerr << "IV generation failed!" << std::endl;
        return false;
    }

    // Write IV at the beginning of the output file
    out.write(reinterpret_cast<char*>(iv.data()), IV_LENGTH);

    std::vector<unsigned char> in_buf(BUFFER_SIZE);
    std::vector<unsigned char> out_buf(BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH); // allow padding

    while (in.read(reinterpret_cast<char*>(in_buf.data()), BUFFER_SIZE) || in.gcount() > 0)
    {
        int read_len = static_cast<int>(in.gcount());
        int encrypted_len = Encrypt(in_buf.data(), read_len, key, iv.data(), out_buf.data());

        out.write(reinterpret_cast<char*>(out_buf.data()), encrypted_len);
    }

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