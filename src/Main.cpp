#include "Main.h"
#include "Storage.h"
#include <wx/valtext.h>
#include "FileEncrypt.h"
#include <filesystem>

static Storage storage;

Main::Main() : ThePier(nullptr, wxID_ANY, window_title, wxPoint(30, 30), wxSize(730, 325), wxDEFAULT_FRAME_STYLE | wxSYSTEM_MENU | wxTAB_TRAVERSAL)
{
	    wxTextValidator validator(wxFILTER_EXCLUDE_CHAR_LIST);
		wxArrayString invalidChars;
		invalidChars.Add(wxT(";"));
		invalidChars.Add(wxT("\r"));
		invalidChars.Add(wxT("\n"));
		validator.SetExcludes(invalidChars);
		SendText->SetValidator(validator);
		
}

void Main::OnAppClose(wxCloseEvent& event)
{
	if (currentPassword != "") // prevent attempting save when not logged in
		if (!currentUser.name.empty())
		{
			std::string userDataFile = "data_" + currentUser.name + ".bin";
			storage.Save(userDataFile);
		}

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
	
	auto& member = storage.GetCurrentChannel().GetMemberByUserId(currentUser.user_id);
	auto m = iMessage(std::time(nullptr), member.member_id, text.ToStdString());
	storage.GetCurrentChannel().messages.push_back(m);
	DisplayMsg(m);

	storage.AppendMessage(storage.GetCurrentChannel(), m, currentUser);

	sendtext->SetFocus();
}

Storage& Main::GetStorage()
{
	return storage;
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
	for (auto& [index, member] : storage.GetCurrentChannel().members)
	{
		ChannelMembers->AppendString(storage.users.at(member.user_id).name);
	}

	SendBtn->Disable();
	SendText->SetFocus();
}

void Main::DisplayMsg(iMessage& msg)
{
	auto& member = storage.GetCurrentChannel().members.find(msg.member_id)->second;
	ChatDisplay->AppendText(msg.FormatToPrint(member.name));
}

bool Main::Login(User user, std::string password)
{
	this->currentUser = user;
	this->currentPassword = password;

	encryption_key.clear();
	encryption_key = MakeKeyFromPassword(currentUser.name + currentPassword, currentUser.name);

	std::string userDataFile = "data_" + currentUser.name + ".bin";
	storage.OpenStorage(userDataFile, encryption_key, currentUser);
	if (storage.channels.empty())
	{
		throw "Not implemented: storage failed to unlock the channels data due to password verification failure.";
	}

	ChannelsBox->Clear();
	for (auto& c : storage.channels)
		ChannelsBox->Append(c.name);
	ChannelsBox->SetSelection(storage.currentChannelIndex);
	auto e = wxCommandEvent();
	OnChannelsBox(e);

	return true; // if storage can be opened 
}

void Main::ReceiveHandler(Channel *ch, iMessage msg)
{
	storage.AppendMessage(*ch, msg, currentUser);
	if (ch->channel_id != storage.GetCurrentChannel().channel_id) return;
	DisplayMsg(msg);
}