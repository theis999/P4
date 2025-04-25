#include "Channel.h"
#include <rpc.h>
#include <sstream>

Channel::Channel(string name) : name(name)
{
	channel_id = channel_id_counter++;
	UuidCreate(&global_id);
}

Channel::Channel(int id, string name, GUID global_id) :
	channel_id(id), name(name), global_id(global_id)
{ }

Member& Channel::GetMemberByUserId(int user_id)
{
	for (auto& [id, member] : members)
		if (member.user_id == user_id)
			return member;
	throw "Member not found with id: " + user_id;
}

string Channel::ToFileString()
{
	std::ostringstream ss;
	RPC_CSTR uuidStr;
	UuidToStringA(&global_id, &uuidStr);

	ss << channel_id << ";" << name << ";" << uuidStr;

	RpcStringFreeA(&uuidStr);
	return ss.str();
}
