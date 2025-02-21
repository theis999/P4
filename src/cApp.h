#pragma once

#include "wx/wx.h"
#include "Main.h"

class cApp : public wxApp
{

private:
	Main* m_frame1 = nullptr;

public:
	virtual bool OnInit() override;
};
