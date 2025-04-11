#pragma once

#include "STDimport.h"

#include "Member.h"
#include "iMessage.h"

using std::vector;


class Channel
{
protected:
	inline static int channel_id_counter = 0;

public:
	string name;
	bool active;
	int channel_id;

	vector<Member> members;
	vector<iMessage> messages;

	Channel(string name, bool active = true);

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
