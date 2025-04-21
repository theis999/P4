#include "Main.h"
#include "Storage.h"

static Storage storage;

Main::Main() : ThePier(nullptr, wxID_ANY, window_title, wxPoint(30, 30), wxSize(730, 325), wxDEFAULT_FRAME_STYLE | wxSYSTEM_MENU | wxTAB_TRAVERSAL)
{
}

void Main::OnAppClose(wxCloseEvent& event)
{
	storage.Save("../data.txt");
	event.Skip();
}

void Main::OnSendTextChange(wxCommandEvent& event)
{
	auto text = SendText->GetValue();
	SendBtn->Enable(text.size() > 0); // simple test if there is anything to send
	event.Skip();
}

void Main::OnSendTextEnter(wxCommandEvent& event)
{
	if (SendBtn->IsEnabled())
		SendHandler(SendText);
}

void Main::OnSend(wxCommandEvent& event)
{
	if (SendBtn->IsEnabled())
		SendHandler(SendText);
}

void Main::SendHandler(wxTextCtrl* sendtext)
{
	auto text = sendtext->GetValue();
	sendtext->Clear();
	
	auto m = iMessage(std::time(nullptr), storage.GetCurrentChannel().GetMemberByUserId(currentUser.user_id).member_id, text.ToStdString());
	storage.GetCurrentChannel().messages.push_back(m);
	DisplayMsg(m);

	storage.AppendMessage(storage.GetCurrentChannel(), m);

	sendtext->SetFocus();
}

void Main::OnChannelsBox(wxCommandEvent& event)
{
	auto item = ChannelsBox->GetStringSelection();
	ChatLabel->SetLabel(item);

	storage.currentChannelIndex = ChannelsBox->GetSelection();
	ChatDisplay->Clear();
	for (auto& m : storage.GetCurrentChannel().messages)
	{
		DisplayMsg(m);
	}
	ChannelMembers->Clear();
	for (auto& m : storage.GetCurrentChannel().members)
	{
		ChannelMembers->AppendString(storage.users.at(m.user_id).name);
	}

	SendBtn->Disable();
	SendText->SetFocus();
}

void Main::DisplayMsg(iMessage& m)
{
	ChatDisplay->AppendText(m.FormatToPrint(storage.GetCurrentChannel().members[m.member_id].name));
}

bool Main::Login(User user, std::string password)
{
	storage.OpenStorage("../data.txt"); // expect the file to be located in the project root
	if (storage.channels.empty())
	{
	}

	ChannelsBox->SetSelection(storage.currentChannelIndex);
	auto e = wxCommandEvent();
	OnChannelsBox(e);

	this->currentUser = user;
	this->currentPassword = password;
	return true; // if storage can be opened 
}
