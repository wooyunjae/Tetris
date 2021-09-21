#pragma once
#include "GraphicsAPIHeader.h"
#include "Scene.h"

class GameBoard;
class SoundPlayer;

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

	void FillRectangle(FLOAT x, FLOAT y, FLOAT w, FLOAT h, ID2D1BitmapBrush* grid);
	ID2D1BitmapBrush* GetBlockKindBitmap(int blockKind);
private:
	// >> 

	// >> 장치 의존
	ID2D1BitmapBrush* mpEmptyGrid;
	ID2D1BitmapBrush* mpTGrid;
	ID2D1BitmapBrush* mpLGrid;
	ID2D1BitmapBrush* mpRLGrid;
	ID2D1BitmapBrush* mpZGrid;
	ID2D1BitmapBrush* mpRZGrid;
	ID2D1BitmapBrush* mpOGrid;
	ID2D1BitmapBrush* mpIGrid;
	
	IDWriteTextFormat* mpText;

	// >> 일반 변수
	unsigned int mWidth;
	unsigned int mHeight;

	GameBoard* mpGameBoard;
	SoundPlayer* mpBlockMoveSound;
};