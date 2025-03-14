#pragma once

#include "sync.h"
//#include <wx/msgdlg.h>

static bool resolveMessageConflictsByOrigin(int clientOrigin, int peerOrigin) // returns true when successful
{
	if (clientOrigin == 0)
	{
		if (peerOrigin == 0)
		{
			return true;
		// END SYNC, we collided on first check, all is good in the world
		}


		// REQUEST MESSAGES FROM peerOrigin
		// ADD MESSAGES TO OUR LIST & HASH THEM
		// HASHES SHOULD NOW MATCH
		
		// if not then something major broke or transit failed
		return false;
	} else 

	if (peerOrigin == 0)
	{
		// GET MESSAGES FROM OUR ORIGIN, SEND TO PEER
		// AWAIT PEER SYNC HASH CHECK FOR ? TIME
		return true;

	} else {
		
		// REQUEST MESSAGES FROM peerOrigin
		// SORT MESSAGES FROM clientOrigin with messages from peerOrigin
		// 
	
	}

	return true;
};

static std::tuple<bool,int,int,std::map<string, int>> findOrigins(std::map<string, int> hashMap, vector<string> clientHashes, vector<string> peerHashes, int global_i) // returns true if Origins are found
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
				return std::make_tuple(true, clientOrigin, peerOrigin, hashMap);
			
			}
		}

		if (local_i < sizePeerHashes)
		{
			auto b = hashMap.insert({peerHashes[local_i], local_i + global_i});
			if (!b.second) // second is false when insertion fails
			{
				peerOrigin = local_i + global_i;
				auto& [key, value] = *b.first; // grab key and value, from the pair where insert failed
				//auto& g = (*b.first).second; // same as above
				clientOrigin = value;
				return std::make_tuple(true, clientOrigin, peerOrigin, hashMap);
			}
		}
	}

	return std::make_tuple(false, 0,0, hashMap); // no Origins found this time.

};


void sync()
{
	// ## THIS FUNCTION SHALL BE MOVED TO BE PART OF CHANNEL, such that network can be used ##

	// DEBUG TBD
 	vector<string> clientHashes = {"3S597R7AA25VYS6MQHROQ6MVZCY8K3AU", "WJ8JVRYJGAFZC6LLSXMQ7TZKTZTBWT3X"};
	vector<string> peerHashes = {"DUDMIY2KGJMHZH3O87JLUXL3TEPTA7QZ", "TJ4Q8IELKACHCDMU7Z8VLUVZ8I1OS2JF"};

	std::map<string, int> hashMap = {};
	int global_i = 0;
	bool OriginsNotFound = true;
	int n = 2; // How many hashes we get at a time
	std::tuple<bool, int, int, std::map<string, int>> x(false,0,0,{});
	while (1)
	{
		// vector<string> clientHashes = FUNCTION TO GET n clientHashes
		// vector<string> peerHashes = FUNCTION TO GET n peerHashes
		x = findOrigins(hashMap, clientHashes, peerHashes, global_i);
		if (std::get<0>(x)) break;

		global_i += n;

		hashMap = std::get<3>(x);

		// DEBUG TBD
		clientHashes = {"8UREPPUAFXROLO1W2GFO1SIFS0D676VZ"};
		peerHashes = {"WJ8JVRYJGAFZC6LLSXMQ7TZKTZTBWT3X"};
	}

	int clientOrigin = std::get<1>(x);
	int peerOrigin = std::get<2>(x);
	
	//wxMessageBox("Client Origin: " + std::to_string(clientOrigin)+ "   Peer Origin: " + std::to_string(peerOrigin), "!"); // DEBUG

	resolveMessageConflictsByOrigin(clientOrigin, peerOrigin);



};