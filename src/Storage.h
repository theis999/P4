#pragma once

#include "STDimport.h"
#include "Channel.h"
#include "User.h"
#include "Member.h"
#include "iMessage.h"

/// <summary>
/// Container for all data in std format
/// </summary>
class Storage
{
public:
	Storage(){ }

	void OpenStorage(string filename);

	vector<Channel> channels; // channels i am part of
	vector<User> users; // users i know, which means they are a member of a channel i am a part of

	string ToFileString();


};
