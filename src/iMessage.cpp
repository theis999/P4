#include "iMessage.h"
#include <openssl/sha.h>
#include <cstring>
#include <sstream>
#include <format>
#include "time.h"

iMessage::iMessage(time_t _timestamp, int _member_id, string _text) :
	timestamp(_timestamp), member_id(_member_id), text(_text)
{
	computeHash();
}

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, shash _hash) :
	timestamp(_timestamp), member_id(_member_id), text(_text), hash(_hash)
{
	computeHash();
}

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, shash _hash, shash _chainHash) :
	timestamp(_timestamp), member_id(_member_id), text(_text), hash(_hash), chainHash(_chainHash)
{
	computeHash();
}

string iMessage::FormatToPrint(string user_name)
{
	char timefmtstring[std::size("yyyy-mm-ddThh:mm:ssZ")];
	std::strftime(std::data(timefmtstring), std::size(timefmtstring), "%H:%M", std::localtime(&timestamp));

	return std::format("{} {}: {}\n", string(timefmtstring), user_name, text);
}

std::string iMessage::to_sc_sep_str()
{
	std::string out;

	// Append timestamp;
	out.append(std::format("{};", this->timestamp));
	// Append member id
	out.append(std::format("{};", this->member_id)); // NEEDS TO BE A GUID
	// Append hash
	out.append(std::format("{};", *(reinterpret_cast<uint32_t*>(this->hash.data()))));
	// Append chainhash
	out.append(std::format("{};", *(reinterpret_cast<uint32_t*>(this->chainHash.data()))));
	// Append text last.
	out.append(this->text);

	return out;
}

iMessage iMessage::from_str(std::string iMessageString)
{
	std::vector<std::string> iMsgFields{};
	std::stringstream ss(iMessageString);
	std::string field{0};
	while (std::getline(ss, field, ';'))
	{
		iMsgFields.push_back(field);
	}

	time_t timestamp = stoi(iMsgFields[0]);
	int memb_id = stoi(iMsgFields[1]); // Should be a GUID?
	uint32_t h = stoul(iMsgFields[2]);
	iMessage::shash hash = *(reinterpret_cast<iMessage::shash*>(&h));
	h = stoul(iMsgFields[3]);
	iMessage::shash chainhash = *(reinterpret_cast<iMessage::shash*>(&h));
	std::string text = iMsgFields[4];
	
	// Construct an iMessage.
	iMessage msg(timestamp, memb_id, text, hash, chainhash);

	return msg;
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
