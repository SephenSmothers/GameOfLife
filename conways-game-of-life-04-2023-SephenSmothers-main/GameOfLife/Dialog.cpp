#include "Dialog.h"
#include "wx/wx.h"
#include "Settings.h"

wxBEGIN_EVENT_TABLE(Dialog, wxDialog)

EVT_COLOURPICKER_CHANGED(10101, Dialog::ChangeDeadColor)
EVT_COLOURPICKER_CHANGED(10102, Dialog::ChangeLivingColor)
EVT_SPINCTRL(10103, Dialog::ChangeGridSize)
EVT_SPINCTRL(10104, Dialog::ChangeInterval)


wxEND_EVENT_TABLE();

Dialog::Dialog(wxWindow* window, Settings* dialogSettings) 
    : wxDialog(window,wxID_ANY,"Settings")
{
    _dialogSettings = dialogSettings;
	mainBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainBox);

    intervalBox = new wxBoxSizer(wxHORIZONTAL);
    gridSizeBox = new wxBoxSizer(wxHORIZONTAL);
    dCellColorBox = new wxBoxSizer(wxHORIZONTAL);
    lCellColorBox = new wxBoxSizer(wxHORIZONTAL);

    intervalLabel = new wxStaticText(this, wxID_ANY, "Speed\t");
    gridSizeLabel = new wxStaticText(this, wxID_ANY, "Grid Size\t");
    dCellColorLabel = new wxStaticText(this, wxID_ANY, "Dead Cell Color\t");
    lCellColorLabel = new wxStaticText(this, wxID_ANY, "Living Cell Color\t");

    wxSizer* verifyCtrl = CreateButtonSizer(wxOK | wxCANCEL);

    mainBox->Add(intervalBox);
    mainBox->Add(gridSizeBox);
    mainBox->Add(dCellColorBox);
    mainBox->Add(lCellColorBox);
    mainBox->Add(verifyCtrl);

    intervalBox->Add(intervalLabel);
    intervalBox->Add(intervalCtrl);

    gridSizeBox->Add(gridSizeLabel);
    gridSizeBox->Add(gridCtrl);

    dCellColorBox->Add(dCellColorLabel);
    dCellColorBox->Add(dCellCtrl);

    lCellColorBox->Add(lCellColorLabel);
    lCellColorBox->Add(lCellCtrl);

    dCellCtrl->SetColour(_dialogSettings->GetDeadCellColor());
    lCellCtrl->SetColour(_dialogSettings->GetLivingCellColor());
    gridCtrl->SetValue(_dialogSettings->gridSize);
    intervalCtrl->SetValue(_dialogSettings->interval);
}	

Dialog::~Dialog()
{

}

void Dialog::ChangeGridSize(wxSpinEvent& event)
{
    _dialogSettings->gridSize = event.GetValue();
    _dialogSettings->SaveData();
}
void Dialog::ChangeInterval(wxSpinEvent& event)
{
    _dialogSettings->interval = event.GetValue();
    _dialogSettings->SaveData();
}

void Dialog::ChangeDeadColor(wxColourPickerEvent& event)
{
    _dialogSettings->SetDeadCellColor(event.GetColour());
    _dialogSettings->SaveData();
}

void Dialog::ChangeLivingColor(wxColourPickerEvent& event)
{
    _dialogSettings->SetLivingCellColor(event.GetColour());
    _dialogSettings->SaveData();
}
