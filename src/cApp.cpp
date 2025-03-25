#include "cApp.h"

wxIMPLEMENT_APP(cApp);

bool cApp::OnInit()
{
	if (!wxApp::OnInit())
	{
		return false;
	};

	m_frame1 = new Main();
	m_frame1->Show();
	m_frame1->DoLogin();

	return true;
};