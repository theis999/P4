#pragma once
#include <array>
#include <openssl/sha.h>
#include "STDimport.h"

using std::array;
/// <summary>
/// A single message in a chat channel
/// </summary>
class iMessage
{
public:
	struct shash : public array<std::byte, 32>
	{

	};

	iMessage(time_t timestamp, int member_id, string text, string signature);
	iMessage(time_t timestamp, int member_id, string text, string signature, shash hash);
	iMessage(time_t timestamp, int member_id, string text, string signature, shash hash, shash chainhash);

	string FormatToPrint(string user_name);

	time_t timestamp; // When the message was sent
	int member_id; // Who sent the message
	string text; // What was sent

	shash hash;
	shash chainHash; //missing implementation
	string signature;

	static string hash_to_string2(iMessage::shash bytes);
	static shash string_to_hash2(string hexidecimal);

	std::string to_sc_sep_str();
	static iMessage from_str(std::string iMessageString);
	bool hasHash();
	void computeHash();
	void computeChainHash(iMessage::shash PreviousHash);
	bool operator==(const iMessage& rhs) const;
};
