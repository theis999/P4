#include "iMessage.h"
#include <openssl/sha.h>
#include <cstring>
#include <sstream>

iMessage::iMessage(time_t _timestamp, int _member_id, string _text) :
	timestamp(_timestamp), member_id(_member_id), text(_text)
{
	computeHash();
}

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, std::array<std::byte, 32> _hash) :
	timestamp(_timestamp), member_id(_member_id), text(_text), hash(_hash)
{
	computeHash();
}

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, std::array<std::byte, 32> _hash, std::array<std::byte, 32> _chainHash) :
	timestamp(_timestamp), member_id(_member_id), text(_text), hash(_hash), chainHash(_chainHash)
{
	computeHash();
}

bool iMessage::hasHash()
{
	return !hash.empty();
}

void iMessage::computeHash()
{
	unsigned char hash_buffer[SHA256_DIGEST_LENGTH];

	// Create a string combining timestamp, member_id, and text
	std::ostringstream data_stream;
	data_stream << timestamp << "|" << member_id << "|" << text;
	std::string data = data_stream.str();

	// Compute hash and copy to hash
	SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash_buffer);
	std::memcpy(hash.data(), hash_buffer, SHA256_DIGEST_LENGTH);
}
