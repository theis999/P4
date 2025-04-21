#include "Member.h"

Member::Member(int user_id, int channel_id, string name) : user_id(user_id), channel_id(channel_id), name(name)
{
	member_id = id_counter++;
}

string Member::ToFileString()
{
	return std::to_string(channel_id) + ";"
		+ std::to_string(user_id) + ";"
		+ name;
}
