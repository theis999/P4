#include "User.h"

typedef GUID UUID;

User::User(GUID unique_id, int user_id, string name): unique_id(unique_id), user_id(user_id), name(name) { }
User::User(GUID unique_id, int user_id, string name, std::byte public_key[]) : User(unique_id, user_id, name)
{
	for (int i = 0; i < 64; i++)
		this->public_key[i] = public_key[i];
}
User::User(GUID unique_id, int user_id, string name, std::byte public_key[], std::byte IPv4[]) : User(unique_id, user_id, name, public_key)
{
	for (int i = 0; i < 4; i++)
		this->IPv4[i] = IPv4[i];
}
User::User(GUID unique_id, int user_id, string name, std::byte public_key[], std::byte IPv4[], vector<Channel> channels) : User(unique_id, user_id, name, public_key)
{
	for (auto channel : channels)
	{
		Member m = {user_id, channel.channel_id, name};
		channel.members.push_back(m);
		channel_membership cm = {{m.member_id, channel}};
		memberships.push_back(cm);
	}
}

string User::ToFileString()
{
	return "ERROR - not implemented";
}
