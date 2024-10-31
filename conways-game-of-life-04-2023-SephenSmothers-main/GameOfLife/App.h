#pragma once

#include "wx/wx.h"

class MainWindow;

class App : public wxApp
{
private:
	MainWindow* mMainWindowPanel = nullptr;

public:
	App();
	~App();
	virtual bool OnInit();
};

