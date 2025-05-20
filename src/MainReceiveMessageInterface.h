#pragma once
#include "Channel.h"
#include "iMessage.h"

#include "Storage.h"
class MainReceiveMessageInterface
{
public:
	virtual void ReceiveHandler(Channel& ch, iMessage msg) = 0;
	virtual Storage& GetStorage() = 0;
	virtual User& GetCurrentUser() = 0;

};