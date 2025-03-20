#pragma once

#include "STDimport.h"


//static bool resolveMessageConflictsByOrigin(int clientOrigin, int peerOrigin);
// std::tuple<bool, int, int, std::map<iMessage::shash, int>> findOrigins(std::map<iMessage::shash, int> hashMap, vector<iMessage::shash> clientHashes, vector<iMessage::shash> peerHashes, int global_i);
/*
class sync
{
public:
	sync(){}

	vector<string> clientHashes;
	vector<string> peerHashes;
};
*/

void sync();