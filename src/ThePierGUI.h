///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-16-g4b3fa447)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class ThePier
///////////////////////////////////////////////////////////////////////////////
class ThePier : public wxFrame
{
	private:

	protected:
		wxPanel* ChannelsPanel;
		wxStaticText* ChannelsPanelLabelTitle;
		wxStaticLine* m_staticline4;
		wxListBox* ChannelsBox;
		wxPanel* ChatPanel;
		wxStaticText* ChatLabel;
		wxStaticLine* m_staticline5;
		wxTextCtrl* ChatDisplay;
		wxTextCtrl* SendText;
		wxButton* SendBtn;
		wxPanel* ChannelData;
		wxStaticText* CurrentUserLabel;
		wxButton* LoginButton;
		wxStaticLine* m_staticline3;
		wxListBox* ChannelMembers;

		// Virtual event handlers, override them in your derived class
		virtual void OnAppClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnChannelsBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void RunTest( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendTextChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSend( wxCommandEvent& event ) { event.Skip(); }
		virtual void LoginButtonClick( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxButton* test_button;

		ThePier( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 710,325 ), long style = wxCAPTION|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ThePier();

};

///////////////////////////////////////////////////////////////////////////////
/// Class LoginForm
///////////////////////////////////////////////////////////////////////////////
class LoginForm : public wxDialog
{
	private:

	protected:
		wxStaticText* DisplayNameLabel;
		wxStaticText* PasscodeLabel;

		// Virtual event handlers, override them in your derived class
		virtual void OnLoginInput( wxCommandEvent& event ) { event.Skip(); }
		virtual void TryLogin( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoginInputPassword( wxCommandEvent& event ) { event.Skip(); }
		virtual void ClickCreateNewUser( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxTextCtrl* UsernameTextBox;
		wxTextCtrl* PasswordTextBox;
		wxButton* CreateNewUserButton;
		wxButton* LoginButton;
		wxStaticText* WrongMessageLabel;

		LoginForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Login"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 320,175 ), long style = wxDEFAULT_DIALOG_STYLE );

		~LoginForm();

};

