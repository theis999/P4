#include "Message.h"

Message::Message(std::time_t timestamp, wxString username, wxString messagecontent) {
	this->MessageContent = messagecontent.Clone();
	this->TimeSinceEpoch = timestamp;
	this->UserName = username.Clone();
}

wxString Message::FormatToPrint() {
	char timefmtstring[std::size("yyyy-mm-ddThh:mm:ssZ")];
	std::strftime(std::data(timefmtstring), std::size(timefmtstring), "%H:%M", std::localtime(&TimeSinceEpoch));

	this->OutputMessage = wxString::Format("%s %s: %s\n", wxString::FromAscii(timefmtstring), UserName, MessageContent);
	return OutputMessage;
}

