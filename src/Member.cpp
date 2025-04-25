#include "Member.h"

Member::Member(int user_id, int channel_id, string name) : user_id(user_id), channel_id(channel_id), name(name)
{
	member_id = id_counter++;
}

Member::Member(int user_id, int channel_id, string name, int _member_id) : Member(user_id, channel_id, name)
{
	member_id = _member_id;
	id_counter = id_counter <= _member_id ? _member_id + 1 : id_counter;
}

string Member::ToFileString()
{
	return std::to_string(member_id) + ";"
		+ std::to_string(channel_id) + ";"
		+ std::to_string(user_id) + ";"
		+ name;
}
