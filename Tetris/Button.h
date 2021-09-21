#pragma once
#include <string>
#include <functional>
#include "GraphicsAPIHeader.h"


class Button
{
public:
	Button(HWND parent, HINSTANCE hInstance, std::wstring name, RECT rect);
	~Button();

	void Render();
	void Update();
	void SetArea(RECT rect);
	void SetArea(LONG x, LONG y, LONG width, LONG height);
	void SetBitmap(PCWSTR uri, int x, int y);
	void SetAction(std::function<void()> action);
	void ToAction();
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	enum class eState {
		IDLE,
		ON_MOUSE,
		CLICKED
	};
	
	HWND mParent;
	HWND mHwnd;
	std::wstring mName;

	ID2D1HwndRenderTarget* mpRenderTarget;
	ID2D1Bitmap* mpImage;

	RECT mRect;

	eState meState;
	bool isClicked;
	std::function<void()> mAction;
};