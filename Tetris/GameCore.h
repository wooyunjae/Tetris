#pragma once

#include "GraphicsAPIHeader.h"

class Scene;

class GameCore
{
public:
	GameCore();
	virtual ~GameCore();
	HRESULT Initialize(HINSTANCE hInstance);
	void RunGameLoop();
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	// >> 장치 독립
	HWND mhwnd;
	ID2D1Factory* mpD2DFactory;
	IWICImagingFactory* mpWICFactory;

	// >> 일반 변수
	bool isRun;
};

