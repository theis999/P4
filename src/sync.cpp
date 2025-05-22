#pragma once

#include "channel.h"
#include <wx/msgdlg.h>
#include <cstring>
#include <sstream>
#include "Storage.h"
#include "Protocol.h"
#include "MainReceiveMessageInterface.h"
//Storage.h must be includd after wx/msgdlg.h, if not then the program won't compile and errors with a ton of syntax errors

bool Channel::resolveMessageConflictsByOrigin(int clientOrigin, int peerOrigin, Member& memb, User& sender, Storage& storage) // returns true when successful
{
	if (clientOrigin == 0)
	{
		if (peerOrigin == 0)
		{
			return true;
			// END SYNC, we collided on first hash, all is good in the world
		}


		// REQUEST MESSAGES FROM peerOrigin

		//vector<iMessage> payload = {}; // !! MAKE THESE THE INCOMING MESSAGES !!
		auto imsg = this->messages.end() - clientOrigin - 1;
		auto& msg = *imsg;
		vector<iMessage> payload = PierProtocol::SendMSGRequest(*this, memb, msg.chainHash, sender, storage);


		// DEBUG/TEST
		/*extern Storage peerTestStorage;
		payload.push_back(peerTestStorage.GetCurrentChannel().messages[3]);
		payload.push_back(peerTestStorage.GetCurrentChannel().messages[4]);
		payload.push_back(peerTestStorage.GetCurrentChannel().messages[5]);
		*/


		// ADD MESSAGES TO OUR LIST -- already done by receive handler
		/*for (iMessage m : payload)
		{
			this->messages.push_back(m);
		}*/

		// HASHES SHOULD NOW MATCH
		// if not then something major broke or transit failed
		//bool x = std::equal(peerTestStorage.GetCurrentChannel().messages.begin(), peerTestStorage.GetCurrentChannel().messages.end(), this->messages.begin());
		//bool x = (peerTestStorage.GetCurrentChannel().messages == this->messages);


		return true;
	}
	else

		if (peerOrigin == 0)
		{
			// GET MESSAGES FROM OUR ORIGIN
			vector<iMessage> payload = {};
			for (int i = this->messages.size() - clientOrigin; i < this->messages.size(); i++)
			{
				payload.push_back(this->messages[i]);
			}

			//PierProtocol::SendMSGMulti(*this,memb, payload, sender, storage);

			return true;

		}
		else
		{

			// REQUEST MESSAGES FROM peerOrigin
			auto imsg = this->messages.end() - clientOrigin - 1;
			auto& msg = *imsg;
			vector<iMessage> peerMessages = PierProtocol::SendMSGRequest(*this, memb, msg.chainHash, sender, storage);
			map<iMessage::shash, iMessage> map;
			for (auto& m : peerMessages) map.insert({m.hash, m});

			for (int i = this->messages.size() - clientOrigin; i < this->messages.size(); i++)
			{
				auto& m = this->messages[i];
				map.insert({m.hash, m});
			}


			vector<iMessage> incoming_messages;
			for (auto it = map.begin(); it != map.end(); ++it)
			{
				incoming_messages.push_back(it->second);
			}

			std::sort(incoming_messages.begin(), incoming_messages.end(), [](iMessage& a, iMessage& b)
{
	return a.timestamp < b.timestamp;
 });

			this->messages.erase(this->messages.end() - clientOrigin - 1);
			for (iMessage msg : incoming_messages)
			{
				iMessage::shash tempShash = messages.back().chainHash;
				msg.computeChainHash(tempShash);
				messages.push_back(msg);

			}

			// SORT MESSAGES FROM clientOrigin with messages from peerOrigin
			// 

		}

	return true;	
};

Channel::syncOutput Channel::findOrigins(std::map<iMessage::shash, int>& hashMap, vector<iMessage::shash> clientHashes, vector<iMessage::shash> peerHashes, int global_i)
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


void Channel::sync(Member& memb, User &sender, Storage &storage)
{
	
	//vector<iMessage::shash> peerHashes = {}; // TBD once function to get n peerHashes is implemented

	std::map<iMessage::shash, int> hashMap = {};
	int global_i = 0;
	bool OriginsNotFound = true;
	int n = 2; // How many hashes we get at a time
	Channel::syncOutput x{false, 0, 0};
	while (1)
	{

		vector<iMessage::shash> peerHashes = PierProtocol::SendSHASHRequest(*this, memb, global_i, n, sender, storage);

		// DEBUG/TEST
		//extern Storage peerTestStorage;
		//peerHashes.push_back(peerTestStorage.GetCurrentChannel().messages[5].hash);
		//peerHashes.push_back(peerTestStorage.GetCurrentChannel().messages[4].hash);
		//peerHashes.push_back(peerTestStorage.GetCurrentChannel().messages[3].hash);
		//peerHashes.push_back(peerTestStorage.GetCurrentChannel().messages[2].hash);
		//peerHashes.push_back(peerTestStorage.GetCurrentChannel().messages[1].hash);
		//peerHashes.push_back(peerTestStorage.GetCurrentChannel().messages[0].hash);


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

	resolveMessageConflictsByOrigin(clientOrigin, peerOrigin,memb,sender,storage);



}

