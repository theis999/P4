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
			ChannelsBox->AppendString(channel.name);
	}
}

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
	wxMessageBox(text, "Send text enter");

	auto msg = Message(std::time(nullptr), wxString::FromAscii("TestUserName"), text).FormatToPrint();

	ChatDisplay->AppendText(msg);
}

void Main::OnChannelsBox(wxCommandEvent& event) {
	//auto sel = event.GetSelection();
	auto item = ChannelsBox->GetStringSelection();
	ChatLabel->SetLabel(item);
}
