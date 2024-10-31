#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings
{
	unsigned int lRed;
	unsigned int lBlue;
	unsigned int lGreen;
	unsigned int lAlpha;

	unsigned int dRed;
	unsigned int dBlue;
	unsigned int dGreen;
	unsigned int dAlpha;

	int gridSize;
	int interval;

	bool showNeigbors = false;
	bool mIsFinite;

	Settings()
	{
		lRed = 128;
		lBlue = 128;
		lGreen = 128;
		lAlpha = 255;

		dRed = 255;
		dBlue = 255;
		dGreen = 255;
		dAlpha = 255;

		gridSize = 15;
		interval = 50;
	}

	Settings(wxColor _deadColor, wxColor _livingColor, int _gridSize, int _interval) 
	{
		dRed = _deadColor.GetRed();
		dBlue = _deadColor.GetBlue();
		dGreen = _deadColor.GetGreen();
		dAlpha = _deadColor.GetAlpha();

		lRed = _livingColor.GetRed();
		lBlue = _livingColor.GetBlue();
		lGreen = _livingColor.GetGreen();
		lAlpha = _livingColor.GetAlpha();

		gridSize = _gridSize;
		interval = _interval;
	}

	void ResetSettings()
	{
		lRed = 128;
		lBlue = 128;
		lGreen = 128;
		lAlpha = 255;

		dRed = 255;
		dBlue = 255;
		dGreen = 255;
		dAlpha = 255;

		gridSize = 15;
		interval = 50;

		SaveData();
	}

	void SetDeadCellColor(wxColor color) 
	{
		dRed = color.GetRed();
		dBlue = color.GetBlue();
		dGreen = color.GetGreen();
		dAlpha = color.GetAlpha();
	}

	void SetLivingCellColor(wxColor color)
	{
		lRed = color.GetRed();
		lBlue = color.GetBlue();
		lGreen = color.GetGreen();
		lAlpha = color.GetAlpha();
	}

	wxColor GetDeadCellColor()
	{
		wxColor color(dRed, dGreen, dBlue, dAlpha);
		return color;
	}

	wxColor GetLivingCellColor()
	{
		wxColor color(lRed, lGreen, lBlue, lAlpha);
		return color;
	}

	void SaveData()
	{
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}

	void LoadData()
	{
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}
};
