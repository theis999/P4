#pragma once

#include "STDimport.h"

/// <summary>
/// A single message in a chat channel
/// </summary>
class Message
{
public:
	Message(time_t timestamp, int member_id, string text);
	Message(time_t timestamp, int member_id, string text, vector<std::byte> hash);

	time_t timestamp; // When the message was sent
	int member_id; // Who sent the message
	string text; // What was sent

	vector<std::byte> hash;

	bool hasHash();
};
