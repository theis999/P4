#pragma once

#include "channel.h"
#include <wx/msgdlg.h>
#include <cstring>
#include <sstream>

bool Channel::resolveMessageConflictsByOrigin(int clientOrigin, int peerOrigin) // returns true when successful
{
	if (clientOrigin == 0)
	{
		if (peerOrigin == 0)
		{
			return true;
		// END SYNC, we collided on first hash, all is good in the world
		}


		// REQUEST MESSAGES FROM peerOrigin

		vector<iMessage> payload = {}; // !! MAKE THESE THE INCOMING MESSAGES !!

		// ADD MESSAGES TO OUR LIST
		for (iMessage m : payload)
		{
			this->messages.push_back(m);
		}

		// HASHES SHOULD NOW MATCH
		// if not then something major broke or transit failed
		return false;
	} else 

	if (peerOrigin == 0)
	{
		// GET MESSAGES FROM OUR ORIGIN
		vector<iMessage> payload = {};
		for (int i = this->messages.size() - clientOrigin; i < this->messages.size(); i++)
		{
			payload.push_back(this->messages[i]);
		}

		// SEND payload TO PEER
		
		// AWAIT PEER SYNC HASH CHECK FOR ? TIME
		return true;

	} else {
		
		// REQUEST MESSAGES FROM peerOrigin
		// SORT MESSAGES FROM clientOrigin with messages from peerOrigin
		// 
	
	}

	return true;
};

Channel::syncOutput Channel::findOrigins(std::map<iMessage::shash, int>&hashMap, vector<iMessage::shash> clientHashes, vector<iMessage::shash> peerHashes, int global_i)
{
	int clientOrigin;
	int peerOrigin;
	int sizeClientHashes = clientHashes.size();
	int sizePeerHashes = peerHashes.size();

	for (int local_i = 0; local_i < std::max(sizeClientHashes, sizePeerHashes); local_i++)
	{ 
		if (local_i < sizeClientHashes)
		{
			auto a = hashMap.insert({clientHashes[local_i], local_i + global_i});
			if (!a.second) // second is false when insertion fails
			{
				clientOrigin = local_i + global_i;
				auto& [key, value] = *a.first; // grab key and value, from the pair where insert failed
				peerOrigin = value;
				return syncOutput(true, clientOrigin, peerOrigin);
			
			}
		}

		if (local_i < sizePeerHashes)
		{
			auto b = hashMap.insert({peerHashes[local_i], local_i + global_i});
			if (!b.second) // second is false when insertion fails
			{
				peerOrigin = local_i + global_i;
				auto& [key, value] = *b.first; // grab key and value, from the pair where insert failed
				clientOrigin = value;
				return syncOutput(true, clientOrigin, peerOrigin);
			}
		}
	}

	
	return syncOutput(false, 0, 0); // no Origins found this time.

};

iMessage::shash Channel::computeTestHash(std::string input)
{
	unsigned char hash_buffer[SHA256_DIGEST_LENGTH];

	std::ostringstream data_stream{};
	data_stream << input;
	std::string data = data_stream.str();

	// Compute hash and copy to hash
	SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash_buffer);
	iMessage::shash o;
	return o;
};



void Channel::sync()
{
	
	vector<iMessage::shash> peerHashes = {}; // TBD once function to get n peerHashes is implemented


	std::map<iMessage::shash, int> hashMap = {};
	int global_i = 0;
	bool OriginsNotFound = true;
	int n = 2; // How many hashes we get at a time
	Channel::syncOutput x{false, 0, 0};
	while (1)
	{

		// vector<iMesssage:shash> peerHashes = FUNCTION TO GET n peerHashes




		// FUNCTION TO GET n clientHashes
		vector<iMessage::shash> clientHashes = {};

		for (int i = this->messages.size() - global_i - 1; i >= 0 && i >= this->messages.size() - global_i - n; --i)
		{
			clientHashes.push_back(this->messages[i].hash);
		}

		x = findOrigins(hashMap, clientHashes, peerHashes, global_i);
		if (x.isFinished) break;

		global_i += n;

	}

	int clientOrigin = x.clientOrigin;
	int peerOrigin = x.peerOrigin;

	//wxMessageBox("Client Origin: " + std::to_string(clientOrigin)+ "   Peer Origin: " + std::to_string(peerOrigin), "!"); // DEBUG

	resolveMessageConflictsByOrigin(clientOrigin, peerOrigin);



}

