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
	// >> ��ġ ����
	HWND mhwnd;
	ID2D1Factory* mpD2DFactory;
	IWICImagingFactory* mpWICFactory;

	// >> �Ϲ� ����
	bool isRun;
};

