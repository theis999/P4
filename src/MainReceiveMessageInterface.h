#pragma once

class Channel;
class User;
class Storage;
class iMessage;
class MainReceiveMessageInterface
{
public:
	virtual void ReceiveHandler(Channel& ch, iMessage msg) = 0;
	virtual Storage& GetStorage() = 0;
	virtual User& GetCurrentUser() = 0;
	virtual void ReprintChat(Channel& ch) = 0;

};