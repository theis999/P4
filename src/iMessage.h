#pragma once
#include <array>
#include <openssl/sha.h>
#include "STDimport.h"

/// <summary>
/// A single message in a chat channel
/// </summary>
class iMessage
{
public:
	iMessage(time_t timestamp, int member_id, string text);
	iMessage(time_t timestamp, int member_id, string text, std::array<std::byte, 32> hash);
	iMessage(time_t timestamp, int member_id, string text, std::array<std::byte, 32> hash, std::array<std::byte, 32> chainHash);


	time_t timestamp; // When the message was sent
	int member_id; // Who sent the message
	string text; // What was sent

	std::array<std::byte, 32> hash;
	std::array<std::byte, 32> chainHash; //missing implementation

	bool hasHash();
	void computeHash();
};
