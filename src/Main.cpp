#include "Main.h"
#include <wx/msgdlg.h>
#include "Storage.h"
#include "MessageLogger.h" 
#include "Message.h"
#include <ctime>
#include <wx/dir.h>
#include <wx/filename.h>

Main::Main()
    : ThePier(nullptr, wxID_ANY, window_title, wxPoint(30, 30), wxSize(620, 325),
              wxDEFAULT_FRAME_STYLE | wxSYSTEM_MENU | wxTAB_TRAVERSAL)
{
    // Load channels from storage.
    Storage storage{};
    storage.OpenStorage("../data.txt"); // Expects the file to be located in the project root.
    if (!storage.channels.empty())
    {
        ChannelsBox->Clear();
        for (auto& channel : storage.channels)
            ChannelsBox->AppendString(channel.name);
    }

    // --- Create a folder for chat history ---
    wxString folderPath = wxT("chat_history");
    if (!wxDirExists(folderPath))
    {
        if (!wxMkdir(folderPath))
        {
            wxMessageBox("Failed to create chat_history folder", "Error");
        }
    }

    // --- Load chat history for the initially selected channel, if any ---
    wxString currentChannel = ChannelsBox->GetStringSelection();
    if (!currentChannel.IsEmpty())
    {
        wxString filePath = folderPath + wxT("/") + currentChannel + wxT("messagedata.txt");
        LoadChatHistoryFromDatabase(ChatDisplay, filePath.ToStdString());
    }
}

void Main::OnSendTextChange(wxCommandEvent& event)
{
    event.Skip();
}

void Main::OnSendTextEnter(wxCommandEvent& event)
{
    auto text = SendText->GetValue();
    wxMessageBox(text, "Send text enter");

    // Append the new message text to the chat display (with a newline).
    ChatDisplay->AppendText(text + "\n");

    // Save the updated chat history to the file corresponding to the selected channel.
    wxString channelName = ChannelsBox->GetStringSelection();
    if (!channelName.IsEmpty())
    {
        wxString folderPath = wxT("chat_history");
        wxString filePath = folderPath + wxT("/") + channelName + wxT("messagedata.txt");
        SaveChatWindowToDatabase(ChatDisplay, filePath.ToStdString());
    }
}

void Main::OnSend(wxCommandEvent& event)
{
    auto text = SendText->GetValue();
    wxMessageBox(text, "Send text click");
    ChatDisplay->AppendText(text + "\n");

    // Save the updated chat history.
    wxString channelName = ChannelsBox->GetStringSelection();
    if (!channelName.IsEmpty())
    {
        wxString folderPath = wxT("chat_history");
        wxString filePath = folderPath + wxT("/") + channelName + wxT("messagedata.txt");
        SaveChatWindowToDatabase(ChatDisplay, filePath.ToStdString());
    }
}

void Main::OnChannelsBox(wxCommandEvent& event)
{
    // When a channel is selected, update the ChatLabel.
    wxString channelName = ChannelsBox->GetStringSelection();
    ChatLabel->SetLabel(channelName);

    // Clear the chat display.
    ChatDisplay->Clear();

    // Load the chat history for the selected channel.
    wxString folderPath = wxT("chat_history");
    wxString filePath = folderPath + wxT("/") + channelName + wxT("messagedata.txt");
    LoadChatHistoryFromDatabase(ChatDisplay, filePath.ToStdString());
}
