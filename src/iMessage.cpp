#include "iMessage.h"
#include <openssl/sha.h>
#include <cstring>
#include <sstream>
#include <format>
#include "time.h"
#include "Signing.h"

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, string _signature) :
	timestamp(_timestamp), member_id(_member_id), text(_text), signature(_signature)
{
	computeHash();
}

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, string _signature, shash _hash) :
	timestamp(_timestamp), member_id(_member_id), text(_text), signature(_signature), hash(_hash)
{
	computeHash();
}

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, string _signature, shash _hash, shash _chainHash) :
	timestamp(_timestamp), member_id(_member_id), text(_text), signature(_signature), hash(_hash), chainHash(_chainHash)
{
	computeHash();
}

string iMessage::FormatToPrint(string user_name)
{
	char timefmtstring[std::size("yyyy-mm-ddThh:mm:ssZ")];
	std::strftime(std::data(timefmtstring), std::size(timefmtstring), "%H:%M", std::localtime(&timestamp));

	return std::format("{} {}: {}\n", string(timefmtstring), user_name, text);
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
	data_stream << timestamp << member_id << text;
	std::string data = data_stream.str();

	// Compute hash and copy to hash
	SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash_buffer);
	std::memcpy(hash.data(), hash_buffer, SHA256_DIGEST_LENGTH);
}

bool iMessage::operator==(const iMessage& rhs) const
{
	return hash == rhs.hash;
}
