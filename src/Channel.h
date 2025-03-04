#pragma once

#include "STDimport.h"

#include "Member.h"
#include "iMessage.h"

using std::vector;

class Channel
{
protected:
	inline static int channel_id_counter = 0;

public:
	string name;
	bool active;
	int channel_id;

	vector<Member> members;
	vector<Message> messages;

	Channel(string name, bool active = true);

	string ToFileString();
};
