#pragma once
#include "wx/wx.h"
#include "Settings.h"
class DrawingPanel :
    public wxPanel
{
private:
    Settings* drawingSettings = nullptr;
    std::vector<std::vector<bool>>& mGameBoardRef;
    std::vector<std::vector<int>>& mNeighborsRef;

public:
    DrawingPanel(wxFrame* parent,Settings* drawSettings, std::vector<std::vector<bool>>& gameBoardRef, std::vector<std::vector<int>>& neighborsRef);
    ~DrawingPanel();
    void OnPaint(wxPaintEvent& paintEvent);
    void OnLeftClick(wxMouseEvent& event);

    wxDECLARE_EVENT_TABLE();
};

