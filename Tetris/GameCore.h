#pragma once

#include "GraphicsAPIHeader.h"

class GameBoard;

class GameCore
{
public:
	GameCore();
	virtual ~GameCore();
	HRESULT Initialize(HINSTANCE hInstance);
	void RunGameLoop();
private:
	HRESULT CreateDeviceSource();
	void DiscardDeviceResources();
	void Update();
	void Draw();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HRESULT CreateBitmapGridBlock(ID2D1HwndRenderTarget*& renderTarget, ID2D1BitmapBrush*& gridBitmap, D2D1::ColorF::Enum color, bool isTwoTone);

private:
	// >> 장치 독립
	HWND mhwnd;
	ID2D1Factory* mpD2DFactory;

	// >> 장치 의존
	// 더블 버퍼링을 하는 렌더타겟
	ID2D1HwndRenderTarget* mpMainRenderTarget;
	ID2D1BitmapBrush* mpEmptyGrid;
	ID2D1BitmapBrush* mpTGrid;
	ID2D1BitmapBrush* mpLGrid;
	ID2D1BitmapBrush* mpRLGrid;
	ID2D1BitmapBrush* mpZGrid;
	ID2D1BitmapBrush* mpRZGrid;
	ID2D1BitmapBrush* mpOGrid;
	ID2D1BitmapBrush* mpIGrid;

private:
	// >> 일반 변수
	unsigned int mWidth;
	unsigned int mHeight;
	bool isRun;

	GameBoard* mpGame;
};

