#include "Main.h"
#include <wx/msgdlg.h>
#include "Storage.h"

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
	auto text = SendText->GetValue();
	wxMessageBox(text, "Send text enter");
	ChatBox->AppendString(text);
}
void Main::OnSend(wxCommandEvent& event) {
	auto text = SendText->GetValue();
	wxMessageBox(text, "Send text click");
	ChatBox->AppendString(text);
}

void Main::OnChannelsBox(wxCommandEvent& event)
{
	//auto sel = event.GetSelection();
	auto item = ChannelsBox->GetStringSelection();
	ChatLabel->SetLabel(item);
}
