#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"

class Dialog :
    public wxDialog
{
private:
    wxBoxSizer* mainBox;
    wxBoxSizer* intervalBox = nullptr;
    wxBoxSizer* gridSizeBox = nullptr;
    wxBoxSizer* dCellColorBox = nullptr;
    wxBoxSizer* lCellColorBox = nullptr;

    wxStaticText* intervalLabel = nullptr;
    wxStaticText* gridSizeLabel = nullptr;
    wxStaticText* dCellColorLabel = nullptr;
    wxStaticText* lCellColorLabel = nullptr;

    wxColourPickerCtrl* dCellCtrl = new wxColourPickerCtrl(this, 10101);
    wxColourPickerCtrl* lCellCtrl = new wxColourPickerCtrl(this, 10102);
    wxSpinCtrl* gridCtrl = new wxSpinCtrl(this, 10103);
    wxSpinCtrl* intervalCtrl = new wxSpinCtrl(this, 10104);

    Settings* _dialogSettings = nullptr;

public:
    Dialog(wxWindow* window, Settings* dialogSettings);
    ~Dialog();
    void ChangeGridSize(wxSpinEvent& event);
    void ChangeInterval(wxSpinEvent& event);
    void ChangeDeadColor(wxColourPickerEvent&);
    void ChangeLivingColor(wxColourPickerEvent& event);

    wxDECLARE_EVENT_TABLE();
};

