#include "cApp.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit()
{
	if (!wxApp::OnInit())
	{
		return false;
	};

	m_frame1 = new Main();
	m_frame1->SetIcon(*(new wxIcon(wxT("boat.ico"), wxBITMAP_TYPE_ICO)));
	m_frame1->Show();

	return true;
};