#pragma once
#include "GraphicsAPIHeader.h"
#include "Scene.h"

class GameBoard;

class GameBoardScene : public Scene
{
public:
	GameBoardScene(HWND hwnd, HINSTANCE hInstance);
	virtual ~GameBoardScene();

	HRESULT Initialize(HINSTANCE hInstance) override;
	void Update() override;
	void Render() override;

private:
	void DiscardDeviceResources();
	HRESULT CreateDeviceSource();
	HRESULT CreateBitmapGridBlock(ID2D1HwndRenderTarget*& renderTarget, ID2D1BitmapBrush*& gridBitmap, D2D1::ColorF::Enum color, bool isTwoTone);

private:
	// >> ��ġ ����
	ID2D1BitmapBrush* mpEmptyGrid;
	ID2D1BitmapBrush* mpTGrid;
	ID2D1BitmapBrush* mpLGrid;
	ID2D1BitmapBrush* mpRLGrid;
	ID2D1BitmapBrush* mpZGrid;
	ID2D1BitmapBrush* mpRZGrid;
	ID2D1BitmapBrush* mpOGrid;
	ID2D1BitmapBrush* mpIGrid;

	// >> �Ϲ� ����
	unsigned int mWidth;
	unsigned int mHeight;

	GameBoard* mpGameBoard;
};

