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
	HWND mhwnd;

	// >> 일반 변수
	bool isRun;
};

