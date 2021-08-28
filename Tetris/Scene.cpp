#include <assert.h>
#include "Scene.h"
#include "Macro.h"
Scene::Scene(HWND hwnd)
	: mHwnd(hwnd)
	, mpMainRenderTarget(nullptr)
	, mXpos(0)
	, mYpos(0)
	, mWidth(480)
	, mHeight(640)
{
}

Scene::~Scene()
{
	SAFE_RELEASE(mpMainRenderTarget);
}

void Scene::AdjustToCenter()
{
	// 화면의 중앙
	RECT desktopWindow;
	// GetDesktopWindow()함수는 데스크톱 화면의 핸들을 가져온다.
	GetWindowRect(GetDesktopWindow(), &desktopWindow);

	int x = desktopWindow.right / 2;
	int y = desktopWindow.bottom / 2;

	RECT rt = { 0,0,480,640 };
	AdjustWindowRect(&rt, WS_CAPTION | WS_SYSMENU, false);
	MoveWindow(mHwnd, x - (rt.right - rt.left) / 2,
		y - (rt.bottom - rt.top) / 2,
		rt.right - rt.left,
		rt.bottom - rt.top,
		true
	);
	mpMainRenderTarget->Resize(D2D1::SizeU(mWidth, mHeight));
}

void Scene::SetPos(unsigned int x, unsigned int y)
{
	mXpos = x;
	mYpos = y;
}

void Scene::SetArea(unsigned int width, unsigned int height)
{
	mWidth = width;
	mHeight = height;
}
