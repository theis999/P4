#include "User.h"
#include <sstream>
#include <fstream>
#include <algorithm>

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
		channel.members.insert({m.member_id, m});
		channel_membership cm = {{m.member_id, channel}};
		memberships.push_back(cm);
	}
}

string User::ToFileString() const
{
	std::ostringstream ss;
	RPC_CSTR uuidStr;
	UuidToStringA(&unique_id, &uuidStr);

	ss << user_id << ";" << name << ";" << uuidStr;

	RpcStringFreeA(&uuidStr);
	return ss.str();
}

void User::SaveToFile(const std::string& filepath) const
{
	std::ofstream out(filepath, std::ios::app);
	if (out.is_open())
	{
		out << ToFileString() << std::endl;
		out.close();
	}
}

std::optional<User> User::LoadUserByName(const std::string& username, const std::string& filepath)
{
	std::ifstream in(filepath);
	if (!in.is_open()) return std::nullopt;

	std::string line;
	std::string lowerInput = makeLowercase(username);

	while (std::getline(in, line))
	{
		std::istringstream ss(line);
		std::string idStr, name, guidStr;

		if (std::getline(ss, idStr, ';') && std::getline(ss, name, ';') && std::getline(ss, guidStr))
		{
			if (makeLowercase(name) == lowerInput)
			{
				int id = std::stoi(idStr);
				GUID g;
				UuidFromStringA((RPC_CSTR)guidStr.c_str(), &g);
				return User(g, id, name);
			}
		}
	}
	return std::nullopt;
}

std::string makeLowercase(const std::string& str)
{
	std::string lower = str;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	return lower;
}