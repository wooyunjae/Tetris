#pragma once
#include "GraphicsAPIHeader.h"
#include "Scene.h"

class GameBoard;

class GameBoardScene : public Scene
{
public:
	GameBoardScene(HWND hwnd, ID2D1Factory* D2DFactory);
	virtual ~GameBoardScene();

	HRESULT Initialize() override;
	void Update() override;
	void Render() override;

private:
	void DiscardDeviceResources();
	HRESULT CreateDeviceSource();
	HRESULT CreateBitmapGridBlock(ID2D1HwndRenderTarget*& renderTarget, ID2D1BitmapBrush*& gridBitmap, D2D1::ColorF::Enum color, bool isTwoTone);

private:
	// >> 장치 의존
	ID2D1BitmapBrush* mpEmptyGrid;
	ID2D1BitmapBrush* mpTGrid;
	ID2D1BitmapBrush* mpLGrid;
	ID2D1BitmapBrush* mpRLGrid;
	ID2D1BitmapBrush* mpZGrid;
	ID2D1BitmapBrush* mpRZGrid;
	ID2D1BitmapBrush* mpOGrid;
	ID2D1BitmapBrush* mpIGrid;

	// >> 일반 변수
	unsigned int mWidth;
	unsigned int mHeight;

	GameBoard* mpGameBoard;
};

