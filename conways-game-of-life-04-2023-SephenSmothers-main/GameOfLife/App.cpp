#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() {
	mMainWindowPanel = new MainWindow();
	mMainWindowPanel->Show();
	return true;
}