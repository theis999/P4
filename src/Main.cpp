#include "Main.h"
#include <wx/msgdlg.h>
#include "Storage.h"
#include "Message.h"

Main::Main() : ThePier(nullptr, wxID_ANY, window_title, wxPoint(30, 30), wxSize(620, 325), wxDEFAULT_FRAME_STYLE | wxSYSTEM_MENU | wxTAB_TRAVERSAL)
{
	Storage storage{};
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

	Message* msg = new Message(std::time(nullptr), wxString::FromAscii("TestUserName"), text);
	auto msgout = msg->FormatToPrint();

	ChatDisplay->AppendText(msgout);
}

void Main::OnChannelsBox(wxCommandEvent& event) {
	//auto sel = event.GetSelection();
	auto item = ChannelsBox->GetStringSelection();
	ChatLabel->SetLabel(item);
}
