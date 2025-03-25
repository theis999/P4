#pragma once
#pragma comment(lib, "Rpcrt4.lib")

#include "STDimport.h"
#include <guiddef.h>
#include <rpc.h>

#include "Member.h"
#include "Channel.h"

/// <summary>
/// A user represent a client setup, each user can be member of many channels.
/// </summary>
class User
{
protected:
	inline static int user_id_counter = 0;

public:
	GUID unique_id; // id of a user for external usage
	int user_id; // id of a user for internal usage

	string name; // default name for the user

	std::byte public_key[64] = {};
	std::byte IPv4[4] = {}; // sockaddr_storage might be better, https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms740504(v=vs.85)?redirectedfrom=MSDN

	struct channel_membership : public pair<int, Channel> { };
	vector<channel_membership> memberships{};


	User() = default;
	User(GUID unique_id, int user_id, string name);
	User(GUID unique_id, int user_id, string name, std::byte public_key[]);
	User(GUID unique_id, int user_id, string name, std::byte public_key[], std::byte IPv4[]);
	User(GUID unique_id, int user_id, string name, std::byte public_key[], std::byte IPv4[], vector<Channel> channels);

	string ToFileString() const;
	void SaveToFile(const std::string& filepath) const;

	static inline User CreateUser(string _name)
	{
		UUID g;
		auto a = UuidCreate(&g);
		return User(g, 0, _name);
	}

	static std::optional<User> LoadUserByName(const std::string& username, const std::string& filepath);
};

std::string makeLowercase(const std::string& str);

