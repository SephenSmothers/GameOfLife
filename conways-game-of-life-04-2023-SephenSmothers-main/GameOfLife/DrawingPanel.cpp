#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)

EVT_PAINT(DrawingPanel::OnPaint, this)
EVT_LEFT_UP(DrawingPanel::OnLeftClick, this)

wxEND_EVENT_TABLE();

DrawingPanel::DrawingPanel(wxFrame* parent,Settings* drawSettings, std::vector<std::vector<bool>>& gameBoardRef, std::vector<std::vector<int>>& neighborsRef)
	: wxPanel(parent, wxID_ANY, wxPoint(0,0)), mGameBoardRef(gameBoardRef), mNeighborsRef(neighborsRef)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetDoubleBuffered(true);
	drawingSettings = drawSettings;
}

DrawingPanel::~DrawingPanel()
{

}

void DrawingPanel::OnPaint(wxPaintEvent& paintEvent)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* graphicsContext = wxGraphicsContext::Create(dc);

	if (!graphicsContext){
		return;
	}

	graphicsContext->SetPen(*wxBLACK);
	
	wxSize panelSize = GetSize();

	graphicsContext->SetFont(wxFontInfo(8), *wxRED);
	

	for (int i = 0; i < drawingSettings->gridSize; i++)
	{
		int cellWidth = panelSize.GetWidth() / drawingSettings->gridSize;
		int cellHeight = panelSize.GetHeight() / drawingSettings->gridSize;

		for (int j = 0; j < drawingSettings->gridSize; j++)
		{
			if (mGameBoardRef[i][j] == true)
			{
				graphicsContext->SetBrush(drawingSettings->GetLivingCellColor());
			}
			else
			{
				graphicsContext->SetBrush(drawingSettings->GetDeadCellColor());
			}
			graphicsContext->DrawRectangle(j * cellWidth, i * cellHeight, cellWidth, cellHeight);

			wxString neighborInfo = std::to_string(mNeighborsRef[i][j]);
			double textWidth, textHeight;
			graphicsContext->GetTextExtent(neighborInfo, &textWidth, &textHeight);
			if (drawingSettings->showNeigbors && mNeighborsRef[i][j] != 0)
			{
				graphicsContext->DrawText(neighborInfo, (j * cellWidth) + (cellWidth - textWidth) / 2, (i * cellHeight) + (cellHeight - textHeight) / 2);
			}
		}
	}
	delete graphicsContext;
}

void DrawingPanel::OnLeftClick(wxMouseEvent& event)
{
	int pointX = event.GetX();
	int pointY = event.GetY();
	wxSize panelSize = GetSize();
	int cellWidth = panelSize.GetWidth() / drawingSettings->gridSize;
	int cellHeight = panelSize.GetHeight() / drawingSettings->gridSize;

	if (mGameBoardRef[pointY / cellHeight][pointX /cellWidth] == true)
	{
		mGameBoardRef[pointY / cellHeight][pointX / cellWidth] = false;
	}
	else if(mGameBoardRef[pointY / cellHeight][pointX / cellWidth] == false)
	{
		mGameBoardRef[pointY / cellHeight][pointX / cellWidth] = true;
	}

	Refresh();
}
