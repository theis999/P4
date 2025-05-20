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

	iMessage(time_t timestamp, int member_id, string text);
	iMessage(time_t timestamp, int member_id, string text, shash hash);
	iMessage(time_t timestamp, int member_id, string text, shash hash, shash chainHash);

	string FormatToPrint(string user_name);

	time_t timestamp; // When the message was sent
	int member_id; // Who sent the message
	string text; // What was sent

	shash hash;
	shash chainHash; //missing implementation

	std::string to_sc_sep_str();
	static iMessage from_str(std::string iMessageString);
	bool hasHash();
	void computeHash();
	bool operator==(const iMessage& rhs) const;
};
