#include "Channel.h"

Channel::Channel(string name, bool active) : name(name), active(active)
{
	//members{};
	//messages{};

	channel_id = channel_id_counter++;
}

string Channel::ToFileString()
{
	return string();
}
