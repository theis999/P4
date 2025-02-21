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
		wxListBox* ChatBox;
		wxTextCtrl* SendText;
		wxButton* SendBtn;

		// Virtual event handlers, override them in your derived class
		virtual void OnChannelsBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendTextChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSendTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSend( wxCommandEvent& event ) { event.Skip(); }


	public:

		ThePier( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCAPTION|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ThePier();

};

