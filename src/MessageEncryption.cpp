#include "MessageEncryption.h"

std::string MessageEncryption::base64_encode(const std::vector<unsigned char>& buffer)
{
    BIO* bio, * b64;
    BUF_MEM* bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // no newline
    BIO_write(bio, buffer.data(), buffer.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);
    return result;
}

std::vector<unsigned char> MessageEncryption::base64_decode(const std::string& encoded)
{
    BIO* bio, * b64;
    int len = encoded.size();
    std::vector<unsigned char> buffer(len);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(encoded.data(), len);
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // no newline
    int decoded_len = BIO_read(bio, buffer.data(), len);
    if (decoded_len < 0) throw std::runtime_error("Base64 decode failed");
    buffer.resize(decoded_len);
    BIO_free_all(bio);
    return buffer;
}

void MessageEncryption::derive_key_iv(const std::string& password, std::vector<unsigned char>& key, std::vector<unsigned char>& iv)
{
    key.resize(32); // AES-256
    iv.resize(16);  // AES block size
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    const EVP_MD* dgst = EVP_sha256();
    if (!EVP_BytesToKey(cipher, dgst, nullptr, reinterpret_cast<const unsigned char*>(password.data()), password.size(), 1, key.data(), iv.data()))
        throw std::runtime_error("EVP_BytesToKey failed");
}

// Encrypt
std::string MessageEncryption::encrypt_string(const std::string& plaintext, const std::string& password)
{
    std::vector<unsigned char> key, iv;
    derive_key_iv(password, key, iv);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Encrypt context failed");

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("EncryptInit failed");

    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
    int len, ciphertext_len;

    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size()) != 1)
        throw std::runtime_error("EncryptUpdate failed");
    ciphertext_len = len;

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
        throw std::runtime_error("EncryptFinal failed");
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);
    return base64_encode(ciphertext);
}

// Decrypt
std::string MessageEncryption::decrypt_string(const std::string& b64ciphertext, const std::string& password)
{
    std::vector<unsigned char> ciphertext = base64_decode(b64ciphertext);
    std::vector<unsigned char> key, iv;
    derive_key_iv(password, key, iv);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Decrypt context failed");

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1)
        throw std::runtime_error("DecryptInit failed");

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len, plaintext_len;

    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1)
        throw std::runtime_error("DecryptUpdate failed");
    plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
        throw std::runtime_error("DecryptFinal failed");
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);
    return std::string(plaintext.begin(), plaintext.end());
}
