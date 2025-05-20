///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-16-g4b3fa447)
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

	ChatPanelSizer->Add( ChatLabel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );


	ChatPanelSizer->Add( bSizer13, 1, wxEXPAND, 5 );

	m_staticline5 = new wxStaticLine( ChatPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	ChatPanelSizer->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* ChatSizer;
	ChatSizer = new wxBoxSizer( wxHORIZONTAL );

	ChatSizer->SetMinSize( wxSize( 299,-1 ) );
	ChatDisplay = new wxTextCtrl( ChatPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	ChatDisplay->SetMinSize( wxSize( 400,180 ) );
	ChatDisplay->SetMaxSize( wxSize( 600,180 ) );

	ChatSizer->Add( ChatDisplay, 0, wxALL|wxEXPAND, 5 );

	test_button = new wxButton( ChatPanel, wxID_ANY, _("Run test"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
	ChatSizer->Add( test_button, 0, wxALL, 5 );


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

	ChannelData = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	CurrentUserLabel = new wxStaticText( ChannelData, wxID_ANY, _("User: Not logged in"), wxDefaultPosition, wxSize( 110,20 ), 0 );
	CurrentUserLabel->Wrap( -1 );
	CurrentUserLabel->SetFont( wxFont( 9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	bSizer11->Add( CurrentUserLabel, 0, wxALL, 5 );

	LoginButton = new wxButton( ChannelData, wxID_ANY, _("Login"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( LoginButton, 0, wxALL|wxEXPAND, 5 );

	m_staticline3 = new wxStaticLine( ChannelData, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer11->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );

	ChannelMembers = new wxListBox( ChannelData, wxID_ANY, wxDefaultPosition, wxSize( 105,165 ), 0, NULL, 0 );
	bSizer11->Add( ChannelMembers, 0, wxALL, 5 );


	ChannelData->SetSizer( bSizer11 );
	ChannelData->Layout();
	bSizer11->Fit( ChannelData );
	MainSizer->Add( ChannelData, 1, wxEXPAND|wxLEFT, 5 );


	this->SetSizer( MainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ThePier::OnAppClose ) );
	ChannelsBox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ThePier::OnChannelsBox ), NULL, this );
	//test_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::RunTest ), NULL, this );
	SendText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ThePier::OnSendTextChange ), NULL, this );
	SendText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ThePier::OnSendTextEnter ), NULL, this );
	SendBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::OnSend ), NULL, this );
	LoginButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::LoginButtonClick ), NULL, this );
}

ThePier::~ThePier()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ThePier::OnAppClose ) );
	ChannelsBox->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ThePier::OnChannelsBox ), NULL, this );
	//test_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::RunTest ), NULL, this );
	SendText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ThePier::OnSendTextChange ), NULL, this );
	SendText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ThePier::OnSendTextEnter ), NULL, this );
	SendBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::OnSend ), NULL, this );
	LoginButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThePier::LoginButtonClick ), NULL, this );

}

LoginForm::LoginForm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* LoginSizer;
	LoginSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* InputSizer;
	InputSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* UsernameSizer;
	UsernameSizer = new wxBoxSizer( wxHORIZONTAL );

	DisplayNameLabel = new wxStaticText( this, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	DisplayNameLabel->Wrap( -1 );
	DisplayNameLabel->SetMinSize( wxSize( 60,-1 ) );

	UsernameSizer->Add( DisplayNameLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	UsernameTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	UsernameSizer->Add( UsernameTextBox, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	InputSizer->Add( UsernameSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* PasswordSizer;
	PasswordSizer = new wxBoxSizer( wxHORIZONTAL );

	PasscodeLabel = new wxStaticText( this, wxID_ANY, _("Password: "), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	PasscodeLabel->Wrap( -1 );
	PasscodeLabel->SetMinSize( wxSize( 60,-1 ) );

	PasswordSizer->Add( PasscodeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	PasswordTextBox = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PASSWORD|wxTE_PROCESS_ENTER );
	PasswordSizer->Add( PasswordTextBox, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	InputSizer->Add( PasswordSizer, 1, wxEXPAND, 5 );


	LoginSizer->Add( InputSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* TrySizer;
	TrySizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	CreateNewUserButton = new wxButton( this, wxID_ANY, _("Create User"), wxDefaultPosition, wxDefaultSize, 0 );
	CreateNewUserButton->Enable( false );

	bSizer13->Add( CreateNewUserButton, 0, wxALL, 5 );

	LoginButton = new wxButton( this, wxID_ANY, _("Login"), wxDefaultPosition, wxDefaultSize, 0 );
	LoginButton->Enable( false );

	bSizer13->Add( LoginButton, 0, wxALL, 5 );


	TrySizer->Add( bSizer13, 1, wxALIGN_RIGHT, 5 );

	WrongMessageLabel = new wxStaticText( this, wxID_ANY, _("Wrong username or key. Try again."), wxDefaultPosition, wxDefaultSize, 0 );
	WrongMessageLabel->Wrap( -1 );
	WrongMessageLabel->SetForegroundColour( wxColour( 255, 0, 0 ) );
	WrongMessageLabel->Hide();

	TrySizer->Add( WrongMessageLabel, 0, wxALL, 5 );


	LoginSizer->Add( TrySizer, 1, wxEXPAND, 5 );


	this->SetSizer( LoginSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	UsernameTextBox->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LoginForm::OnLoginInput ), NULL, this );
	UsernameTextBox->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LoginForm::TryLogin ), NULL, this );
	PasswordTextBox->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LoginForm::OnLoginInputPassword ), NULL, this );
	PasswordTextBox->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LoginForm::TryLogin ), NULL, this );
	CreateNewUserButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginForm::ClickCreateNewUser ), NULL, this );
	LoginButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginForm::TryLogin ), NULL, this );
}

LoginForm::~LoginForm()
{
	// Disconnect Events
	UsernameTextBox->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LoginForm::OnLoginInput ), NULL, this );
	UsernameTextBox->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LoginForm::TryLogin ), NULL, this );
	PasswordTextBox->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( LoginForm::OnLoginInputPassword ), NULL, this );
	PasswordTextBox->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( LoginForm::TryLogin ), NULL, this );
	CreateNewUserButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginForm::ClickCreateNewUser ), NULL, this );
	LoginButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginForm::TryLogin ), NULL, this );

}
