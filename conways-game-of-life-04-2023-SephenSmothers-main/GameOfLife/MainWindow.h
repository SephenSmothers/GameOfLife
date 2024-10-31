#pragma once
#include "wx/wx.h"
#include "vector"
#include "Settings.h"

class DrawingPanel;
class MainWindow :
    public wxFrame
{
private:
    int mGenerations;
    int mLivingCells;

    wxMenuBar* mMenuBar = nullptr;
    wxMenu* mFileMenu = nullptr;
    wxMenu* mViewMenu = nullptr;
    wxMenu* mDrawMenu = nullptr; 
    wxMenuItem* mFinite = nullptr;
    wxMenuItem* mToroidal = nullptr;


    wxStatusBar* mStatusBar = nullptr;
    wxToolBar* mToolBar = nullptr;
    DrawingPanel* mDrawPanel = nullptr;
    wxBoxSizer* mResizeBox = nullptr;
    wxTimer* mTimer = nullptr;
    std::vector<std::vector<bool>> mGameBoard;
    std::vector<std::vector<bool>> mSandBox;
    std::vector<std::vector<int>> mNeighbors;
    std::vector<std::vector<int>> mRandomNums;

    std::string filePath = "";

    Settings mainSettings;

public:
    MainWindow();
    ~MainWindow();
    void OnSizeChanged(wxSizeEvent& windowChangeEvent);
    void InitializeGrid();
    void NextGeneration(wxCommandEvent& event);
    void StartTimer(wxTimerEvent& event);
    void ClearBoard(wxCommandEvent& event);
    void Play(wxCommandEvent& event);
    void Pause(wxCommandEvent& event);
    int CalculateNeighbors(int x, int y);
    void DispalyNeighbors();
    void RandomizeCtrl(int seed);
    void RandomizeWithSeedCtrl(int seed);
    void RefreshMenuItems();
    int ToroidalNeighbors(int x, int y);
    
    //Menu Bar Event handlers
    void ToggleNeighbors(wxCommandEvent& event);
    void Randomize(wxCommandEvent& event);
    void RandomizeWithSeed(wxCommandEvent& event);
    void New(wxCommandEvent& event);
    void Open(wxCommandEvent& event);
    void Save(wxCommandEvent& event);
    void SaveAs(wxCommandEvent& event);
    void FileOptions(wxCommandEvent& event);
    void Exit(wxCommandEvent& event);
    void ResetSettings(wxCommandEvent& event);
    void OnFiniteClick(wxCommandEvent& event);
    void OnToroidalClick(wxCommandEvent& event);



    wxDECLARE_EVENT_TABLE();
};

