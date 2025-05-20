#pragma once

#include "STDimport.h"
#include "Channel.h"
#include "User.h"
#include "iMessage.h"

/// <summary>
/// Container for all data in std format
/// </summary>
class Storage
{
	enum datatype
	{
		d_user, d_membership, d_channel
	};
	static inline const vector<string> datatype_names{
		"user", "membership", "channel"
	};
	static inline const map<string, datatype> map_name_to_datatype{
		{"user", d_user}, {"membership", d_membership}, {"channel", d_channel}
	};

public:
	std::vector<unsigned char> encryption_key; //Encryption key

	static bool UserNameExists(const std::string& username, const std::string& filepath);
	static bool CreateUser(const User& user, const std::string& filepath);
	void OpenStorage(string filename, std::vector<unsigned char> encryption_key, User currentUser);

	void AppendMessage(const Channel& c, const iMessage& msg, const User& currentUser);

	vector<Channel> channels; // channels i am part of
	vector<User> users; // users i know, which means they are a member of a channel i am a part of

	int currentChannelIndex = 0;
	Channel& GetCurrentChannel();
	Channel& GetChannel(GUID guid);

	void Save(string filename); //Save the data file of the current user
	string ToFileString();
};
