#pragma once

#include "STDimport.h"

/// <summary>
/// (Channel)member
/// </summary>
class Member
{
protected:
	inline static int id_counter = 0;
public:
	int member_id; // internal reference id number

	int user_id; // link to the members user
	int channel_id; // link to the members channel
	string name; // the display name of the member

	Member(int user_id, int channel_id, string name);
};
