#pragma once

#include "STDimport.h"

#include <guiddef.h>
#include "Member.h"
#include "iMessage.h"

using std::vector;


class Channel
{
protected:
	inline static int channel_id_counter = 0;

public:
	int channel_id; // local id of the channel for fast lookup comparing memberships
	string name; // the display name of the channel
	GUID global_id; // global id of this channel, used for network

	map<int, Member> members;
	vector<iMessage> messages;

	Channel(string name); // minimal constructor
	Channel(int id, string name, GUID global_id); // regular constructor

	Member& GetMemberByUserId(int user_id);

	string ToFileString();

	struct syncOutput
	{
		bool isFinished;
		int clientOrigin, peerOrigin;
	};

	// Sync related functions, found in sync.ccp
	void sync();
	bool resolveMessageConflictsByOrigin(int clientOrigin, int peerOrigin);
	syncOutput findOrigins(std::map<iMessage::shash, int>& hashMap, vector<iMessage::shash> clientHashes, vector<iMessage::shash> peerHashes, int global_i);

};
