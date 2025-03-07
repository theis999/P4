#include "iMessage.h"

iMessage::iMessage(time_t _timestamp, int _member_id, string _text) : 
	timestamp(_timestamp), member_id(_member_id), text(_text)
{ }

iMessage::iMessage(time_t _timestamp, int _member_id, string _text, vector<std::byte> _hash) : 
	timestamp(_timestamp), member_id(_member_id), text(_text), hash(_hash)
{ }

bool iMessage::hasHash()
{
	return !hash.empty();
}
