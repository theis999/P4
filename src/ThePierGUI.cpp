///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-66-g57ca7150)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ThePierGUI.h"

///////////////////////////////////////////////////////////////////////////

ThePier::ThePier( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* MainSizer;
	MainSizer = new wxBoxSizer( wxHORIZONTAL );

	ChannelsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* ChannelsPanelSizer;
	ChannelsPanelSizer = new wxBoxSizer( wxVERTICAL );

	ChannelsPanelLabelTitle = new wxStaticText( ChannelsPanel, wxID_ANY, _("Channels"), wxDefaultPosition, wxDefaultSize, 0 );
	ChannelsPanelLabelTitle->Wrap( -1 );
	ChannelsPanelLabelTitle->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	ChannelsPanelSizer->Add( ChannelsPanelLabelTitle, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticline4 = new wxStaticLine( ChannelsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	ChannelsPanelSizer->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );

	ChannelsBox = new wxListBox( ChannelsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	ChannelsBox->Append( _("Pirates") );
	ChannelsBox->Append( _("Ninjas") );
	ChannelsBox->Append( _("Nerds") );
	ChannelsPanelSizer->Add( ChannelsBox, 0, wxALL, 5 );


	ChannelsPanel->SetSizer( ChannelsPanelSizer );
	ChannelsPanel->Layout();
	MainSizer->Add( ChannelsPanel, 1, wxEXPAND, 5 );

	ChatPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* ChatPanelSizer;
	ChatPanelSizer = new wxBoxSizer( wxVERTICAL );

	ChatLabel = new wxStaticText( ChatPanel, wxID_ANY, _("Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	ChatLabel->Wrap( -1 );
	ChatLabel->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	ChatPanelSizer->Add( ChatLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_staticline5 = new wxStaticLine( ChatPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	ChatPanelSizer->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* ChatSizer;
	ChatSizer = new wxBoxSizer( wxVERTICAL );

	ChatSizer->SetMinSize( wxSize( 299,-1 ) );
	ChatDisplay = new wxTextCtrl( ChatPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	ChatDisplay->SetMinSize( wxSize( 400,180 ) );
	ChatDisplay->SetMaxSize( wxSize( 600,180 ) );

	ChatSizer->Add( ChatDisplay, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 5 );


	ChatPanelSizer->Add( ChatSizer, 1, wxALIGN_LEFT|wxSHAPED, 5 );

	wxBoxSizer* SendMsgSizer;
	SendMsgSizer = new wxBoxSizer( wxHORIZONTAL );

	SendText = new wxTextCtrl( ChatPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER );
	SendText->SetMinSize( wxSize( 400,-1 ) );
	SendText->SetMaxSize( wxSize( 700,-1 ) );

	SendMsgSizer->Add( SendText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	SendBtn = new wxButton( ChatPanel, wxID_ANY, _("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	SendBtn->SetMaxSize( wxSize( 65,-1 ) );

	SendMsgSizer->Add( SendBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	ChatPanelSizer->Add( SendMsgSizer, 1, wxALIGN_LEFT|wxSHAPED, 5 );


	ChatPanel->SetSizer( ChatPanelSizer );
	ChatPanel->Layout();
	ChatPanelSizer->Fit( ChatPanel );
	MainSizer->Add( ChatPanel, 1, wxEXPAND|wxLEFT, 5 );


	this->SetSizer( MainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	ChannelsBox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ThePier::OnChannelsBox ), NULL, this );
	SendText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ThePier::OnSendTextChange ), NULL, this );
	SendText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ThePier::OnSendTextEnter ), NULL, this );
	SendBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::OnSend ), NULL, this );
}

ThePier::~ThePier()
{
	// Disconnect Events
	ChannelsBox->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ThePier::OnChannelsBox ), NULL, this );
	SendText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ThePier::OnSendTextChange ), NULL, this );
	SendText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ThePier::OnSendTextEnter ), NULL, this );
	SendBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::OnSend ), NULL, this );

}
