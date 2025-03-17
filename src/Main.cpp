#include "Main.h"
#include "Storage.h"
#include "Message.h"

static Storage storage;

Main::Main() : ThePier(nullptr, wxID_ANY, window_title, wxPoint(30, 30), wxSize(620, 325), wxDEFAULT_FRAME_STYLE | wxSYSTEM_MENU | wxTAB_TRAVERSAL)
{
	storage.OpenStorage("../data.txt"); // expect the file to be located in the project root
	if (!storage.channels.empty())
	{
		ChannelsBox->Clear();
		for (auto &channel : storage.channels)
		{
			ChannelsBox->AppendString(channel.name);
			channel.members.push_back({Member(0, channel.channel_id, "Test" + channel.name.substr(0, channel.name.size()-1))});
	}
}

	ChannelsBox->SetSelection(storage.currentChannelIndex);
	auto item = ChannelsBox->GetStringSelection();
	ChatLabel->SetLabel(item);
void Main::OnSendTextChange(wxCommandEvent& event)
{
	//wxMessageBox("hej");
	event.Skip();
}
void Main::OnSendTextEnter(wxCommandEvent& event) {
	SendHandler(SendText);
}
void Main::OnSend(wxCommandEvent& event) {
	SendHandler(SendText);
}

// Username handling still needed
void Main::SendHandler(wxTextCtrl* sendtext) {
	auto text = sendtext->GetValue();
	sendtext->Clear();

	auto m = iMessage(std::time(nullptr), 0, text.ToStdString());
	storage.GetCurrentChannel().messages.push_back(m);
	DisplayMsg(m);

	sendtext->SetFocus();
}

void Main::OnChannelsBox(wxCommandEvent& event) {
	auto item = ChannelsBox->GetStringSelection();
	ChatLabel->SetLabel(item);

	storage.currentChannelIndex = ChannelsBox->GetSelection();
	ChatDisplay->Clear();
	for (auto& m : storage.GetCurrentChannel().messages)
	{
		DisplayMsg(m);
	}
}

void Main::DisplayMsg(iMessage& m)
{
	auto msg = Message(m.timestamp, storage.GetCurrentChannel().members[m.member_id].name, wxString(m.text)).FormatToPrint();
	ChatDisplay->AppendText(msg);
}
