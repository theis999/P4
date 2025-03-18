#include "MessageLogger.h"
#include <fstream>
#include <sstream>

void SaveChatWindowToDatabase(wxTextCtrl* chatCtrl, const string& filename)
{
    std::ofstream ofs(filename, std::ios::out); 
    if (ofs.is_open())
    {
        // Get the entire text from the chat control.
        ofs << chatCtrl->GetValue().ToStdString();
        ofs.close();
    }
   
}

void LoadChatHistoryFromDatabase(wxTextCtrl* chatCtrl, const string& filename)
{
    std::ifstream ifs(filename);
    if (ifs.is_open())
    {
        // Read entire file contents.
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        chatCtrl->SetValue(buffer.str());
        ifs.close();
    }
}
