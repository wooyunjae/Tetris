#include <assert.h>
#include "GraphicsAPIHeader.h"
#include "MainScene.h"
#include "Macro.h"
#include "Bitmap.h"
#include "SceneManager.h"
#include "GameBoardScene.h"
#include "Button.h"
#include "Factorys.h"
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
	delete mCloseButton;
	delete mStartButton;
}

HRESULT MainScene::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = S_OK;
	if (!mpMainRenderTarget)
	{
		D2D1_SIZE_U size = D2D1::SizeU(mWidth, mHeight);
		hr = FACTORY->GetFactory()->CreateHwndRenderTarget(
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
			mStartButton = new Button(mHwnd, hInstance, std::wstring(L"StartButton"), rt);
			RECT size;
			GetWindowRect(mHwnd, &size);
			mStartButton->SetArea((size.right - size.left) / 2 - 50, (size.bottom - size.top) / 2 - 100, 100, 50);
			mStartButton->SetBitmap(L".\\StartButton.jpeg", 0, 0);
			std::function<void()> start = std::bind(&SceneManager::SetScene, SM, new GameBoardScene(mHwnd, mhInstance));
			mStartButton->SetAction(start);

			mCloseButton = new Button(mHwnd, hInstance, std::wstring(L"CloseButton"), rt);
			mCloseButton->SetArea((size.right - size.left) / 2 - 50, (size.bottom - size.top) / 2, 100, 50);
			mCloseButton->SetBitmap(L".\\CloseButton.jpeg", 0, 0);
			std::function<void()> close = std::bind(SendMessage, mHwnd, WM_DESTROY, 0, 0);
			mCloseButton->SetAction(close);
		}
	}
	return hr;
}

void MainScene::Update()
{
	mStartButton->Update();
	mCloseButton->Update();
}

void MainScene::Render()
{
	mpMainRenderTarget->BeginDraw();
	D2D1_SIZE_F size = mpMainScreen->GetSize();
	mpMainRenderTarget->DrawBitmap(mpMainScreen, D2D1::RectF(0.0f, 0.0f, size.width, size.height));
	mpMainRenderTarget->EndDraw();
	mStartButton->Render();
	mCloseButton->Render();
}