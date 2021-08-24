#pragma once
#include "GraphicsAPIHeader.h"

class Scene
{
public:
	Scene(HWND hwnd, ID2D1Factory* D2DFactory);
	virtual ~Scene();

	virtual HRESULT Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void AdjustToCenter();
	void SetPos(unsigned int x, unsigned int y);
	void SetArea(unsigned int width, unsigned int height);
protected:
	HWND mHwnd;
	ID2D1Factory* mpCoreGameFactory;
	// >> 장치 의존
	// 더블 버퍼링을 하는 렌더타겟
	ID2D1HwndRenderTarget* mpMainRenderTarget;

	unsigned int mXpos;
	unsigned int mYpos;
	unsigned int mWidth;
	unsigned int mHeight;
};

