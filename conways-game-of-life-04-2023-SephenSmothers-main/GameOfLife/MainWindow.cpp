#define PLAY_BUTTON_ID 10001  
#define PAUSE_BUTTON_ID 10002
#define NEXT_BUTTON_ID 10003
#define TRASH_BUTTON_ID 10004
#define TIMER_BUTTON_ID 10005
#define SETTINGS_ID 10006
#define NEIGHBORS_ID 10007
#define RANDOMIZE_ID 10008
#define RANDOMIZE_SEED_ID 10009
#define NEW_ID 10010
#define SAVE_AS_ID 10011
#define EXIT_ID 10012
#define RESET_SETTINGS 10013
#define FINITE_ID 10014
#define TOROIDAL_ID 10015


#include "MainWindow.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "Dialog.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include <fstream>
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

enum ID {
	NUM1 = 10001,
	NUM2 = 10002
};


wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)

EVT_SIZE(MainWindow::OnSizeChanged, this)
EVT_MENU(NEXT_BUTTON_ID, MainWindow::NextGeneration)
EVT_MENU(TRASH_BUTTON_ID, MainWindow::ClearBoard)
EVT_MENU(PLAY_BUTTON_ID, MainWindow::Play)
EVT_MENU(PAUSE_BUTTON_ID, MainWindow::Pause)
EVT_TIMER(TIMER_BUTTON_ID, MainWindow::StartTimer)
EVT_MENU(NEIGHBORS_ID, MainWindow::ToggleNeighbors)
EVT_MENU(RANDOMIZE_ID, MainWindow::Randomize)
EVT_MENU(RANDOMIZE_SEED_ID, MainWindow::RandomizeWithSeed)
EVT_MENU(NEW_ID, MainWindow::New)
EVT_MENU(wxID_OPEN, MainWindow::Open)
EVT_MENU(wxID_SAVE, MainWindow::Save)
EVT_MENU(SAVE_AS_ID, MainWindow::SaveAs)
EVT_MENU(SETTINGS_ID, MainWindow::FileOptions)
EVT_MENU(EXIT_ID, MainWindow::Exit)
EVT_MENU(RESET_SETTINGS, MainWindow::ResetSettings)
EVT_MENU(FINITE_ID, MainWindow::OnFiniteClick)
EVT_MENU(TOROIDAL_ID, MainWindow::OnToroidalClick)

wxEND_EVENT_TABLE();

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Conway's Game Of Life", wxPoint(0, 0), wxSize(750, 750))
{
	mainSettings.LoadData();

	//Tool Bar Stuff
	mStatusBar = CreateStatusBar();
	mToolBar = CreateToolBar();

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	mToolBar->AddTool(PLAY_BUTTON_ID, "Play", playIcon);
	mToolBar->AddTool(PAUSE_BUTTON_ID, "Pause", pauseIcon);
	mToolBar->AddTool(NEXT_BUTTON_ID, "Next", nextIcon);
	mToolBar->AddTool(TRASH_BUTTON_ID, "Trash", trashIcon);
	mToolBar->Realize();

	int number = NUM1 + 2;

	//Settings Stuff

	//Menu Bar Stuff
	mMenuBar = new wxMenuBar();
	mFileMenu = new wxMenu();
	mViewMenu = new wxMenu();
	mDrawMenu = new wxMenu();
	mFinite = new wxMenuItem(mViewMenu, FINITE_ID, "Finite Grid", "", wxITEM_CHECK);
	mToroidal = new wxMenuItem(mViewMenu, TOROIDAL_ID, "Toroidal Grid", "", wxITEM_CHECK);
	mFinite->SetCheckable(true);
	mToroidal->SetCheckable(true);
	SetMenuBar(mMenuBar);

	mMenuBar->Append(mFileMenu, "File");
	mMenuBar->Append(mViewMenu, "View");
	mMenuBar->Append(mDrawMenu, "Draw");

	mFileMenu->Append(NEW_ID, "New");
	mFileMenu->Append(wxID_OPEN, "Open");
	mFileMenu->Append(wxID_SAVE, "Save");
	mFileMenu->Append(SAVE_AS_ID, "Save As");
	mFileMenu->Append(SETTINGS_ID, "Settings");
	mFileMenu->Append(RESET_SETTINGS, "Reset Settings");
	mFileMenu->Append(EXIT_ID, "Exit");

	mViewMenu->Append(NEIGHBORS_ID, "Neighbors");
	mViewMenu->Append(mFinite);
	mViewMenu->Append(mToroidal);

	mDrawMenu->Append(RANDOMIZE_ID, "Randomize");
	mDrawMenu->Append(RANDOMIZE_SEED_ID, "Randomize With Seed");

	//Drawing Panel Stuff
	mDrawPanel = new DrawingPanel(this, &mainSettings, mGameBoard, mNeighbors);
	mResizeBox = new wxBoxSizer(wxVERTICAL);
	mTimer = new wxTimer(this, TIMER_BUTTON_ID);
	mResizeBox->Add(mDrawPanel, 1, wxEXPAND | wxALL);
	SetSizer(mResizeBox);
	InitializeGrid();
	RefreshMenuItems();
	this->Layout();
}

MainWindow::~MainWindow()
{
	delete mDrawPanel;
	delete mTimer;
}

void MainWindow::OnSizeChanged(wxSizeEvent& windowChangeEvent)
{
	if (mDrawPanel != nullptr)
	{
		mDrawPanel->SetSize(windowChangeEvent.GetSize());
		mDrawPanel->Refresh();
	}
	windowChangeEvent.Skip();
}

void MainWindow::InitializeGrid()
{
	mGameBoard.resize(mainSettings.gridSize);
	mSandBox.resize(mainSettings.gridSize);
	mNeighbors.resize(mainSettings.gridSize);
	mRandomNums.resize(mainSettings.gridSize);

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		mGameBoard[i].resize(mainSettings.gridSize);
		mSandBox[i].resize(mainSettings.gridSize);
		mNeighbors[i].resize(mainSettings.gridSize);
		mRandomNums[i].resize(mainSettings.gridSize);
	}
}

int MainWindow::CalculateNeighbors(int x, int y)
{
	int neighbors = 0;


	if (mainSettings.mIsFinite == false)
	{
		neighbors = ToroidalNeighbors(x, y);
	}
	else
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (x + i >= 0 && x + i < mGameBoard.size() && y + j >= 0 && y + j < mGameBoard.size())
				{
					if (x + i != x || y + j != y)
					{
						if (mGameBoard[x + i][y + j] == true)
						{
							neighbors++;
						}
					}
				}
			}
		}
	}


	return neighbors;
}

void MainWindow::NextGeneration(wxCommandEvent& event)
{
	int livingNeighbors = 0;

	for (int i = 0; i < mSandBox.size(); i++)
	{
		for (int j = 0; j < mSandBox.size(); j++)
		{
			mSandBox[i][j] = false;
		}
	}
	mLivingCells = 0;

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard[i].size(); j++)
		{
			if (mGameBoard[i][j] == true)
			{
				livingNeighbors = CalculateNeighbors(i, j);


				if (livingNeighbors == 2 || livingNeighbors == 3)
				{
					mSandBox[i][j] = true;
					mLivingCells++;
				}
				else
				{
					mSandBox[i][j] = false;
				}
			}
			else
			{
				if (CalculateNeighbors(i, j) == 3)
				{
					mSandBox[i][j] = true;
					mLivingCells++;
				}
			}
		}
	}
	swap(mSandBox, mGameBoard);

	DispalyNeighbors();

	mGenerations++;
	mStatusBar->SetStatusText("Living Cells: " + std::to_string(mLivingCells) + "\t\t| Generations: " + std::to_string(mGenerations));
	Refresh();
}

void MainWindow::ClearBoard(wxCommandEvent& event)
{
	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard.size(); j++)
		{
			mGameBoard[i][j] = false;
			mSandBox[i][j] = false;
		}
	}

	mGenerations = 0;
	mLivingCells = 0;
	Refresh();
}

void MainWindow::StartTimer(wxTimerEvent& event)
{
	wxCommandEvent _event(PLAY_BUTTON_ID);
	NextGeneration(_event);
}

void MainWindow::Play(wxCommandEvent& event)
{
	mTimer->Start(mainSettings.interval);
}

void MainWindow::Pause(wxCommandEvent& event)
{
	mTimer->Stop();
}

void MainWindow::ToggleNeighbors(wxCommandEvent& event)
{
	mainSettings.showNeigbors = !mainSettings.showNeigbors;

	if (mainSettings.showNeigbors)
	{
		DispalyNeighbors();
	}
	mainSettings.SaveData();
	Refresh();
}

void MainWindow::DispalyNeighbors()
{
	int currentNeighbors;

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard.size(); j++)
		{
			currentNeighbors = CalculateNeighbors(i, j);
			mNeighbors[i][j] = currentNeighbors;
		}
	}
}

void MainWindow::Randomize(wxCommandEvent& event)
{
	RandomizeCtrl(time(NULL));
}

void MainWindow::RandomizeWithSeed(wxCommandEvent& event)
{
	Dialog* options = new Dialog(this, &mainSettings);
	wxBoxSizer* promptBox = new wxBoxSizer(wxVERTICAL);
	SetSizer(promptBox);
	int seed = wxGetNumberFromUser("", "Seed", "Enter Seed For Random Grid", 0, 0, 999999);

	RandomizeWithSeedCtrl(seed);
}

void MainWindow::RandomizeCtrl(int seed)
{
	srand(seed);

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard.size(); j++)
		{
			mGameBoard[i][j] = false;
			mRandomNums[i][j] = 0;
		}
	}

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard.size(); j++)
		{
			mRandomNums[i][j] = rand() % 100;
			if (mRandomNums[i][j] < 30)
			{
				mGameBoard[i][j] = true;
			}
		}
	}
	Refresh();
}

void MainWindow::RandomizeWithSeedCtrl(int seed)
{
	srand(seed);
	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard.size(); j++)
		{
			mGameBoard[i][j] = false;
			mRandomNums[i][j] = 0;
		}
	}

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		for (int j = 0; j < mGameBoard.size(); j++)
		{
			mRandomNums[i][j] = rand() % 100;
			if (mRandomNums[i][j] < 30)
			{
				mGameBoard[i][j] = true;
			}
		}
	}
	Refresh();
}

void MainWindow::New(wxCommandEvent& event)
{

}

void MainWindow::Open(wxCommandEvent& event)
{
	wxFileDialog file(this, "Open Game Of Life", wxEmptyString, wxEmptyString, "Game Of Life Files (*.cells) | *.cells", wxFD_OPEN);
	if (file.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	for (int i = 0; i < mGameBoard.size(); i++)
	{
		mGameBoard[i].clear();
		mGameBoard[i].resize(0);
	}
	mGameBoard.clear();
	mGameBoard.resize(0);

	std::string buffer;
	std::ifstream fileStream;
	int index = 0;
	fileStream.open((std::string)file.GetPath());
	filePath = (std::string)file.GetPath();
	if (fileStream.is_open())
	{
		while (!fileStream.eof())
		{
			std::getline(fileStream, buffer);

			if (buffer.size() == 0)
				break;

			mainSettings.gridSize = buffer.size();
			mGameBoard.resize(mainSettings.gridSize);
			mGameBoard[index].resize(mainSettings.gridSize);

			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*')
				{
					mGameBoard[index][i] = true;
				}
				else
				{
					mGameBoard[index][i] = false;
				}
			}
			index++;
		}

		fileStream.close();
		mDrawPanel->Refresh();
	}
	mainSettings.SaveData();
	InitializeGrid();
	mainSettings.LoadData();
	
	event.Skip();
}

void MainWindow::Save(wxCommandEvent& event)
{
	if (filePath == "")
	{
		wxFileDialog file(this, "Save Game Of Life", wxEmptyString, wxEmptyString, "Game Of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		//filePath = (std::string)file.GetPath();

		if (file.ShowModal() == wxID_CANCEL)
		{
			return;
		}
		std::ofstream fileStream;
		fileStream.open((std::string)file.GetPath());

		if (fileStream.is_open())
		{
			for (int i = 0; i < mGameBoard.size(); i++)
			{
				for (int j = 0; j < mGameBoard[i].size(); j++)
				{
					if (mGameBoard[i][j])
					{
						fileStream << '*';
					}
					else
					{
						fileStream << '.';
					}
				}
				fileStream << '\n';
			}
			fileStream.close();
		}
		event.Skip();
	}

	std::ofstream fileStream;
	fileStream.open(filePath);

	if (fileStream.is_open())
	{
		for (int i = 0; i < mGameBoard.size(); i++)
		{
			for (int j = 0; j < mGameBoard[i].size(); j++)
			{
				if (mGameBoard[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		fileStream.close();
	}

	event.Skip();
}

void MainWindow::SaveAs(wxCommandEvent& event)
{
	wxFileDialog file(this, "Save Game", wxEmptyString, wxEmptyString, "Game Of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (file.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	std::ofstream fileStream;
	fileStream.open((std::string)file.GetPath());
	if (fileStream.is_open())
	{
		for (int i = 0; i < mGameBoard.size(); i++)
		{
			for (int j = 0; j < mGameBoard[i].size(); j++)
			{
				if (mGameBoard[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		filePath = file.GetPath();
		fileStream.close();
	}

	event.Skip();
}

void MainWindow::MainWindow::FileOptions(wxCommandEvent& event)
{
	Dialog* options = new Dialog(this, &mainSettings);

	if (options->ShowModal() == wxID_OK)
	{
		InitializeGrid();
		mDrawPanel->Refresh();
	}
}

void MainWindow::Exit(wxCommandEvent& event)
{
	MainWindow::Close();
}

void MainWindow::ResetSettings(wxCommandEvent& event)
{
	mainSettings.ResetSettings();
	Refresh();
}

void MainWindow::OnFiniteClick(wxCommandEvent& event)
{
	mainSettings.mIsFinite = true;
	RefreshMenuItems();
	mainSettings.SaveData();
	event.Skip();
}

void MainWindow::OnToroidalClick(wxCommandEvent& event)
{
	mainSettings.mIsFinite = false;
	RefreshMenuItems();
	mainSettings.SaveData();
	event.Skip();
}

void MainWindow::RefreshMenuItems()
{
	mFinite->Check(mainSettings.mIsFinite);
	mToroidal->Check(!mainSettings.mIsFinite);
}

int MainWindow::ToroidalNeighbors(int x, int y)
{
	int neighbors = 0;
	int xIndex = 0;
	int yIndex = 0;


	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			xIndex = x + i;
			yIndex = y + j;

			if (xIndex == -1)
			{
				xIndex = mGameBoard.size() - 1;
			}
			else if (xIndex == mGameBoard.size())
			{
				xIndex = 0;
			}

			if (yIndex == -1)
			{
				yIndex = mGameBoard.size() - 1;
			}
			else if (yIndex == mGameBoard.size())
			{
				yIndex = 0;
			}
			if (xIndex >= 0 && xIndex < mGameBoard.size() && yIndex >= 0 && yIndex < mGameBoard.size())
			{
				if (xIndex != x || yIndex != y)
				{
					if (mGameBoard[xIndex][yIndex] == true)
					{
						neighbors++;
					}
				}
			}
		}
	}

	return neighbors;
}