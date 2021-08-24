#include <assert.h>
#include "GameBoardScene.h"
#include "GameBoard.h"
#include "Macro.h"
#include "CST.h"
#include "Color.h"

GameBoardScene::GameBoardScene(HWND hwnd, ID2D1Factory* D2DFactory)
	: Scene(hwnd, D2DFactory)
	, mHeight(640)
	, mWidth(480)
	, mpGameBoard(nullptr)
	, mpEmptyGrid(nullptr)
	, mpTGrid(nullptr)
	, mpLGrid(nullptr)
	, mpRLGrid(nullptr)
	, mpZGrid(nullptr)
	, mpRZGrid(nullptr)
	, mpOGrid(nullptr)
	, mpIGrid(nullptr)
{
	mpGameBoard = new GameBoard();
	if (!SUCCEEDED(Initialize()))
	{
		assert(false);
	}
}

GameBoardScene::~GameBoardScene()
{
	DiscardDeviceResources();
	delete mpGameBoard;
}

HRESULT GameBoardScene::Initialize()
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
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpEmptyGrid, D2D1::ColorF::White, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpTGrid, D2D1::ColorF::Purple, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpLGrid, D2D1::ColorF::Blue, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpRLGrid, D2D1::ColorF::Orange, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpZGrid, D2D1::ColorF::YellowGreen, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpRZGrid, D2D1::ColorF::Red, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpOGrid, D2D1::ColorF::Yellow, true);
		}
		if (SUCCEEDED(hr))
		{
			hr = CreateBitmapGridBlock(mpMainRenderTarget, mpIGrid, D2D1::ColorF::LightSkyBlue, true);
		}
	}

	return hr;
}

void GameBoardScene::Update()
{
	mpGameBoard->Update();
}

void GameBoardScene::Render()
{
	HRESULT hr S_OK;

	hr = CreateDeviceSource();
	if (SUCCEEDED(hr))
	{
		mpMainRenderTarget->BeginDraw();
		mpMainRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		mpMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_RECT_F gameBoardRect = {
			0, 0, CST::BLOCK_SIZE * 10, CST::BLOCK_SIZE * 20
		};
		D2D1_RECT_F scoreBoard = {
			CST::BLOCK_SIZE * 10, 0, static_cast<float>(mWidth), static_cast<float>(mHeight)
		};

		ID2D1SolidColorBrush* gameBoardBrush;
		ID2D1SolidColorBrush* gameScoreBrush;

		//
		// 매 프레임 마다 게임 화면과 스코어를 그림
		// 매 프레임 마다 solid color brush를 생성하는 것은 부하가 적음
		//
		hr = mpMainRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple), &gameBoardBrush);
		if (SUCCEEDED(hr))
		{
			mpMainRenderTarget->FillRectangle(gameBoardRect, gameBoardBrush);
		}
		hr = mpMainRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &gameScoreBrush);
		if (SUCCEEDED(hr))
		{
			mpMainRenderTarget->FillRectangle(scoreBoard, gameScoreBrush);
		}
		int(*board)[CST::WIDTH] = mpGameBoard->GetBoard();

		for (unsigned int y = 0; y < CST::HEIGHT; y++)
		{
			for (unsigned int x = 0; x < CST::WIDTH; x++)
			{
				D2D1_RECT_F rt = {
					static_cast<float>(CST::BLOCK_SIZE * x),
					static_cast<float>(CST::BLOCK_SIZE * y),
					static_cast<float>(CST::BLOCK_SIZE * (x + 1)),
					static_cast<float>(CST::BLOCK_SIZE * (y + 1))
				};
				switch (board[y][x])
				{
				case CST::EMPTY:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpEmptyGrid
					);
					break;
				case CST::T:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpTGrid
					);
					break;
				case CST::L:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpLGrid
					);
					break;
				case CST::RL:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpRLGrid
					);
					break;
				case CST::Z:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpZGrid
					);
					break;
				case CST::RZ:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpRZGrid
					);
					break;
				case CST::O:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpOGrid
					);
					break;
				case CST::I:
					mpMainRenderTarget->FillRectangle(
						rt,
						mpIGrid
					);
					break;
				default:
					assert(false);
				}
			}
		}

		mpMainRenderTarget->EndDraw();
	}
}

void GameBoardScene::DiscardDeviceResources()
{
	SAFE_RELEASE(mpEmptyGrid);
	SAFE_RELEASE(mpTGrid);
	SAFE_RELEASE(mpLGrid);
	SAFE_RELEASE(mpRLGrid);
	SAFE_RELEASE(mpZGrid);
	SAFE_RELEASE(mpRZGrid);
	SAFE_RELEASE(mpOGrid);
	SAFE_RELEASE(mpIGrid);
}

HRESULT GameBoardScene::CreateDeviceSource()
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
	}

	return hr;
}

HRESULT GameBoardScene::CreateBitmapGridBlock(ID2D1HwndRenderTarget*& renderTarget, ID2D1BitmapBrush*& gridBitmap, D2D1::ColorF::Enum color, bool isTwoTone)
{

	HRESULT hr = S_OK;

	// 호환 렌더타겟을 생성.
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	hr = renderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(CST::BLOCK_SIZE, CST::BLOCK_SIZE), &pCompatibleRenderTarget);
	if (SUCCEEDED(hr))
	{
		ID2D1SolidColorBrush* gridColor1 = nullptr;
		ID2D1SolidColorBrush* gridColor2 = nullptr;
		D2D1::ColorF color2(Color::DarkenColor(color, 5));
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &gridColor1);
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(color2, &gridColor2);
		assert(gridColor1 != nullptr);
		assert(gridColor2 != nullptr);
		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, CST::BLOCK_SIZE, CST::BLOCK_SIZE), gridColor1);
			pCompatibleRenderTarget->DrawRectangle(D2D1::RectF(0.0f, 0.0f, CST::BLOCK_SIZE, CST::BLOCK_SIZE), gridColor2);
			if (isTwoTone)
			{
				D2D1_RECT_F rec = {
					CST::BLOCK_SIZE / 8,
					CST::BLOCK_SIZE / 8,
					CST::BLOCK_SIZE * 7 / 8,
					CST::BLOCK_SIZE * 7 / 8
				};
				pCompatibleRenderTarget->FillRectangle(rec, gridColor2);
			}
			hr = pCompatibleRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				DiscardDeviceResources();
			}
			if (SUCCEEDED(hr))
			{
				ID2D1Bitmap* pGridBitmap = nullptr;
				hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);
				if (SUCCEEDED(hr))
				{
					hr = mpMainRenderTarget->CreateBitmapBrush(
						pGridBitmap,
						D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP),
						&gridBitmap
					);
					pGridBitmap->Release();
				}
			}
			gridColor1->Release();
			gridColor2->Release();
		}
		pCompatibleRenderTarget->Release();
	}
	return hr;
}
