#pragma once
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/listbox.h>
#include <wx/stattext.h>

class MessageLogger
{
public:
    static void EnsureChatHistoryFolder();
    static void SaveChatHistory(wxTextCtrl* chatDisplay, const wxString& channelName);
    static void LoadChatHistory(wxTextCtrl* chatDisplay, const wxString& channelName);
};
