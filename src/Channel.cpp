#include "Channel.h"
#include <sstream>
#include "Uuid.h"

Channel::Channel(string name) : name(name)
{
	channel_id = channel_id_counter++;
	global_id = GuidCreate();
}

Channel::Channel(int id, string name, GUID global_id) :
	channel_id(id), name(name), global_id(global_id)
{ }

Member& Channel::GetMemberByUserId(int user_id)
{
	for (auto& [id, member] : members)
		if (member.user_id == user_id)
		{
			return member;
		}
	throw "Member not found with id: " + user_id;
}

string Channel::ToFileString()
{
	std::ostringstream ss;
	ss << channel_id << ";" << name << ";" << GuidToString(global_id);
	return ss.str();
}
