#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <string>
#include "GameBoardScene.h"
#include "GameBoard.h"
#include "Factorys.h"
#include "Macro.h"
#include "CST.h"
#include "Color.h"
#include "SoundPlayer.h"

GameBoardScene::GameBoardScene(HWND hwnd, HINSTANCE hInstance)
	: Scene(hwnd)
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
	if (!SUCCEEDED(Initialize(hInstance)))
	{
		assert(false);
	}
}

GameBoardScene::~GameBoardScene()
{
	DiscardDeviceResources();
	delete mpGameBoard;
	delete mpBlockMoveSound;
}

HRESULT GameBoardScene::Initialize(HINSTANCE hInstance)
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
		if (SUCCEEDED(hr))
		{
			hr = FACTORY->GetDWriteFactory()->CreateTextFormat(
				L"굴림",
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				15,
				L"",
				&mpText
			);
			mpText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			mpText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}

	mpBlockMoveSound = new SoundPlayer();
	WCHAR path[256];
	wcsncpy(path, L".\\water_fall.wav", 256);
	hr = mpBlockMoveSound->LoadAudio(path);
	std::function<void()> water_fall = std::bind(&SoundPlayer::ForcePlayMusic, mpBlockMoveSound);
	mpGameBoard->SetBlockMoveSound(water_fall);
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
		ID2D1SolidColorBrush* textBrush = nullptr;
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
		hr = mpMainRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &textBrush);
		if (SUCCEEDED(hr))
		{
			std::wstring str(L"점수 : ");
			str += std::to_wstring(mpGameBoard->GetScore());
			mpMainRenderTarget->DrawText(
				str.c_str(),
				str.length(),
				mpText,
				D2D1::RectF(CST::BLOCK_SIZE * 10, 0, static_cast<float>(mWidth), 15),
				textBrush
			);
		}


		for (unsigned int i = 0; i < 4; i++)
		{
			int ScaledblockSize = CST::BLOCK_SIZE * 1 / 3;
			FLOAT unitLength = (scoreBoard.right - scoreBoard.left) / 2;
			FLOAT centerX = scoreBoard.left + i % 2 * unitLength + unitLength / 2;
			FLOAT centerY = scoreBoard.top + i / 2 * (unitLength + 10) + unitLength / 2;

			int blockKind = mpGameBoard->GetBlock(i + 1);
			mpMainRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			assert(textBrush != nullptr);
			mpMainRenderTarget->DrawRectangle(D2D1::RectF(scoreBoard.left + i % 2 * unitLength + 5,
				scoreBoard.top + i / 2 * unitLength + 5,
				scoreBoard.left + i % 2 * unitLength + unitLength - 5,
				scoreBoard.top + i / 2 * unitLength + unitLength - 5),
				textBrush,
				3
			);
			mpMainRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(1 / 3.0f, 1 / 3.0f));
			for (unsigned int ii = 0; ii < 8; ii += 2)
			{
				FillRectangle(static_cast<FLOAT>(centerX + ScaledblockSize * CST::BLOCK[blockKind][0][ii] - ScaledblockSize / 2),
					static_cast<FLOAT>(centerY + ScaledblockSize * CST::BLOCK[blockKind][0][ii + 1] - ScaledblockSize / 2),
					CST::BLOCK_SIZE,
					CST::BLOCK_SIZE,
					GetBlockKindBitmap(blockKind)
				);
			}
		}

		mpMainRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		D2D1_RECT_F rect = {
			scoreBoard.left,
			scoreBoard.bottom - (scoreBoard.right - scoreBoard.left),
			scoreBoard.right,
			scoreBoard.bottom
		};
		mpMainRenderTarget->DrawRectangle(&rect, textBrush, 3.0);
		if (mpGameBoard->GetStoredBlock() != CST::EMPTY)
		{
			for (unsigned int ii = 0; ii < 8; ii += 2)
			{
				FillRectangle(static_cast<FLOAT>(rect.left + (rect.right - rect.left) / 2 + CST::BLOCK_SIZE * CST::BLOCK[mpGameBoard->GetStoredBlock()][0][ii] - CST::BLOCK_SIZE / 2),
					static_cast<FLOAT>(rect.top + (rect.right - rect.left) / 2 + CST::BLOCK_SIZE * CST::BLOCK[mpGameBoard->GetStoredBlock()][0][ii + 1] - CST::BLOCK_SIZE / 2),
					CST::BLOCK_SIZE,
					CST::BLOCK_SIZE,
					GetBlockKindBitmap(mpGameBoard->GetStoredBlock())
				);
			}
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
				mpMainRenderTarget->FillRectangle(
					rt,
					GetBlockKindBitmap(board[y][x])
				);
			}
		}
		gameBoardBrush->Release();
		gameScoreBrush->Release();
		textBrush->Release();
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
		hr = FACTORY->GetFactory()->CreateHwndRenderTarget(
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

void GameBoardScene::FillRectangle(FLOAT x, FLOAT y, FLOAT w, FLOAT h, ID2D1BitmapBrush* grid)
{
	D2D1_MATRIX_3X2_F originTrs;
	D2D1_MATRIX_3X2_F trs;
	mpMainRenderTarget->GetTransform(&originTrs);
	trs = originTrs;

	trs = trs * D2D1::Matrix3x2F::Translation(x, y);
	mpMainRenderTarget->SetTransform(&trs);
	mpMainRenderTarget->FillRectangle(
		D2D1::RectF(0, 0, w, h),
		grid
	);
	mpMainRenderTarget->SetTransform(&originTrs);
}

ID2D1BitmapBrush* GameBoardScene::GetBlockKindBitmap(int blockKind)
{
	ID2D1BitmapBrush* bitmap;
	switch (blockKind)
	{
	case CST::EMPTY:
		bitmap = mpEmptyGrid;
		break;
	case CST::T:
		bitmap = mpTGrid;
		break;
	case CST::L:
		bitmap = mpLGrid;
		break;
	case CST::RL:
		bitmap = mpRLGrid;
		break;
	case CST::Z:
		bitmap = mpZGrid;
		break;
	case CST::RZ:
		bitmap = mpRZGrid;
		break;
	case CST::O:
		bitmap = mpOGrid;
		break;
	case CST::I:
		bitmap = mpIGrid;
		break;
	default:
		bitmap = nullptr;
		assert(false);
		break;
	}
	return bitmap;
}
