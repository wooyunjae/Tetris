#include <assert.h>
#include "GraphicsAPIHeader.h"
#include "MainScene.h"
#include "Macro.h"
#include "Bitmap.h"
#include "SceneManager.h"
#include "GameBoardScene.h"
MainScene::MainScene(HWND hwnd, ID2D1Factory* D2DFactory, IWICImagingFactory* wicFactory)
	: Scene(hwnd, D2DFactory)
	, mpWICFactory(wicFactory)
{
	if (!SUCCEEDED(Initialize()))
	{
		assert(false);
	}
}

MainScene::~MainScene()
{
}

HRESULT MainScene::Initialize()
{
	HRESULT hr = S_OK;
	if (!mpMainRenderTarget)
	{
		D2D1_SIZE_U size = D2D1::SizeU(mWidth, mHeight);
		hr = mpCoreGameFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mHwnd, size),
			&mpMainRenderTarget
		);
		hr = Bitmap::LoadBitmapFromFile(mpMainRenderTarget, mpWICFactory, L".\\MainScreen.jpeg", 0, 0, &mpMainScreen);
	}
	return hr;
}

void MainScene::Update()
{
	if (GetAsyncKeyState(0x53))
	{
		GameBoardScene* gb = new GameBoardScene(mHwnd, mpCoreGameFactory);
		SM->SetScene(gb);
		return;
	}
}

void MainScene::Render()
{
	mpMainRenderTarget->BeginDraw();
	D2D1_SIZE_F size = mpMainScreen->GetSize();
	mpMainRenderTarget->DrawBitmap(mpMainScreen, D2D1::RectF(0.0f, 0.0f, size.width, size.height));
	mpMainRenderTarget->EndDraw();
}

void MainScene::SetWICFactory(IWICImagingFactory* wicFactory)
{
	mpWICFactory = wicFactory;
}
