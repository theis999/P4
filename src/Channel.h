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
		//map<iMessage::shash, int> map;
	};


	// Sync related functions, found in sync.ccp
	void sync();
	static bool resolveMessageConflictsByOrigin(int clientOrigin, int peerOrigin);
	std::tuple<bool, int, int/*, std::map<iMessage::shash, int>*/> findOrigins(std::map<iMessage::shash, int>& hashMap, vector<iMessage::shash> clientHashes, vector<iMessage::shash> peerHashes, int global_i);
	//iMessage::shash computeTestHash(std::string input);
};
