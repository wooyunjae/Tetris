#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include "GraphicsAPIHeader.h"
#include "MainScene.h"
#include "Macro.h"
#include "Bitmap.h"
#include "SceneManager.h"
#include "GameBoardScene.h"
#include "Button.h"
#include "Factorys.h"
#include "SoundPlayer.h"


MainScene::MainScene(HWND hwnd, HINSTANCE hInstance)
	: Scene(hwnd)
	, mhInstance(hInstance)
{
	if (!SUCCEEDED(Initialize(hInstance)))
	{
		assert(false);
	}
}

MainScene::~MainScene()
{
	delete mpCloseButton;
	delete mpStartButton;
	delete mpPlayer;
}

HRESULT MainScene::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = S_OK;
	ID2D1Factory* fact = FACTORY->GetFactory();
	if (!mpMainRenderTarget)
	{
		D2D1_SIZE_U size = D2D1::SizeU(mWidth, mHeight);
		
		hr = fact->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&mpMainRenderTarget
		);
		AdjustToCenter();
		if (SUCCEEDED(hr))
		{
			hr = Bitmap::LoadBitmapFromFile(mpMainRenderTarget, FACTORY->GetWICFactory(), L".\\MainScreen.jpeg", 0, 0, &mpMainScreen);
		}
		if (SUCCEEDED(hr))
		{
			RECT rt = { 0,0,0,0 };
			mpStartButton = new Button(mHwnd, hInstance, std::wstring(L"StartButton"), rt);
			RECT size;
			GetWindowRect(mHwnd, &size);
			mpStartButton->SetArea((size.right - size.left) / 2 - 50, (size.bottom - size.top) / 2 - 100, 100, 50);
			mpStartButton->SetBitmap(L".\\StartButton.jpeg", 0, 0);
			std::function<void()> start = std::bind(&SceneManager::SetScene, SM, new GameBoardScene(mHwnd, mhInstance));
			mpStartButton->SetAction(start);

			mpCloseButton = new Button(mHwnd, hInstance, std::wstring(L"CloseButton"), rt);
			mpCloseButton->SetArea((size.right - size.left) / 2 - 50, (size.bottom - size.top) / 2, 100, 50);
			mpCloseButton->SetBitmap(L".\\CloseButton.jpeg", 0, 0);
			std::function<void()> close = std::bind(SendMessage, mHwnd, WM_DESTROY, 0, 0);
			mpCloseButton->SetAction(close);
		}
		if (SUCCEEDED(hr))
		{
			mpPlayer = new SoundPlayer();
			WCHAR path[256];
			wcsncpy(path, L".\\tetris_bgm.wav", 256);
			hr = mpPlayer->LoadAudio(path);
		}
	}
	return hr;
}

void MainScene::Update()
{
	mpStartButton->Update();
	mpCloseButton->Update();
	mpPlayer->PlayMusic();
	if (GetAsyncKeyState(VK_UP))
	{
		mpPlayer->IncreaseVolumn();
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		mpPlayer->DecreaseVolumn();
	}
}

void MainScene::Render()
{
	mpMainRenderTarget->BeginDraw();
	D2D1_SIZE_F size = mpMainScreen->GetSize();
	mpMainRenderTarget->DrawBitmap(mpMainScreen, D2D1::RectF(0.0f, 0.0f, size.width, size.height));
	mpMainRenderTarget->EndDraw();
	mpStartButton->Render();
	mpCloseButton->Render();
}