#include "MessageLogger.h"
#include <wx/msgdlg.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <fstream>
#include <streambuf>

// Ensure chat_history directory exists
void MessageLogger::EnsureChatHistoryFolder()
{
    wxString folderPath = wxT("chat_history");
    if (!wxDirExists(folderPath))
    {
        if (!wxMkdir(folderPath))
        {
            wxMessageBox("Failed to create chat_history folder", "Error");
        }
    }
}

// Save chat history to a file
void MessageLogger::SaveChatHistory(wxTextCtrl* chatDisplay, const wxString& channelName)
{
    wxString filePath = "chat_history/" + channelName + "messagedata.txt";
    std::string filePathStd = filePath.ToStdString();

    // 1) Read existing file content
    std::ifstream inFile(filePathStd, std::ios::in);
    if (!inFile.is_open() && !inFile.good())
    {
        // If no file yet, we can just write everything
        std::ofstream createFile(filePathStd, std::ios::out | std::ios::trunc);
        if (!createFile)
        {
            wxMessageBox("Failed to create chat history file", "Error");
            return;
        }
        createFile << chatDisplay->GetValue().ToStdString();
        return;
    }

    std::string oldContent(
        (std::istreambuf_iterator<char>(inFile)),
         std::istreambuf_iterator<char>()
    );
    inFile.close();

    // 2) Current chat display content
    std::string newContent = chatDisplay->GetValue().ToStdString();

    // 3) Compare old content with new content
    if (newContent.size() >= oldContent.size() &&
        newContent.compare(0, oldContent.size(), oldContent) == 0)
    {
        
        std::string difference = newContent.substr(oldContent.size());

        if (!difference.empty())
        {
            // Append only the new difference
            std::ofstream outFile(filePathStd, std::ios::app);
            if (!outFile)
            {
                wxMessageBox("Failed to append chat history", "Error");
                return;
            }
            outFile << difference;
        }
    }
    else
    {
         // File doesn't match, so we rewrite the entire chat from scratch.
        std::ofstream outFile(filePathStd, std::ios::trunc);
        if (!outFile)
        {
            wxMessageBox("Failed to rewrite chat history", "Error");
            return;
        }
        outFile << newContent;
    }
}

// Load chat history from a file
void MessageLogger::LoadChatHistory(wxTextCtrl* chatDisplay, const wxString& channelName)
{
    wxString filePath = "chat_history/" + channelName + "messagedata.txt";
    std::ifstream file(filePath.ToStdString());

    if (!file)
        return; 

    wxString chatContent;
    std::string line;
    while (std::getline(file, line))
    {
        chatContent += wxString(line) + "\n";
    }

    chatDisplay->SetValue(chatContent);
}
