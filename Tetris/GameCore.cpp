#include <cassert>

#include "GameCore.h"
#include "GameBoard.h"
#include "CST.h"
#include "macro.h"
#include "util.h"
#include "KeyInput.h"

GameCore::GameCore()
	: mWidth(480)
	, mHeight(640)
	, mhwnd(nullptr)
	, mpD2DFactory(nullptr)
	, mpMainRenderTarget(nullptr)
	, isRun(true)
	, mpEmptyGrid(nullptr)
	, mpTGrid(nullptr)
	, mpLGrid(nullptr)
	, mpRLGrid(nullptr)
	, mpZGrid(nullptr)
	, mpRZGrid(nullptr)
	, mpOGrid(nullptr)
	, mpIGrid(nullptr)
{
	mpGame = new GameBoard();
}

GameCore::~GameCore()
{
	delete mpGame;
	// 장치독립
	SAFE_RELEASE(mpD2DFactory);
	// 장치의존
	SAFE_RELEASE(mpMainRenderTarget);
	SAFE_RELEASE(mpEmptyGrid);
	SAFE_RELEASE(mpTGrid);
	SAFE_RELEASE(mpLGrid);
	SAFE_RELEASE(mpRLGrid);
	SAFE_RELEASE(mpZGrid);
	SAFE_RELEASE(mpRZGrid);
	SAFE_RELEASE(mpOGrid);
	SAFE_RELEASE(mpIGrid);
}

HRESULT GameCore::Initialize(HINSTANCE hInstance)
{
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpD2DFactory);

	// Creat window
	WNDCLASSEX wcex = { sizeof(wcex) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = hInstance;
	wcex.hCursor = NULL;
	wcex.lpszClassName = L"Tetris";
	RegisterClassEx(&wcex);

	mhwnd = CreateWindow(
		L"Tetris", L"Tetris_beta",
		WS_CAPTION | WS_SYSMENU,
		0, 0, 0, 0,
		NULL, NULL, hInstance, this
	);
	hr = mhwnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(mhwnd, SW_SHOWNORMAL);
		UpdateWindow(mhwnd);
	}
	return hr;
}

void GameCore::RunGameLoop()
{
	MSG message;

	while (isRun)
	{
		// 메세지 큐에 대기중인 메세지 꺼냄
		// NULL : 현재 쓰레드의 모든 윈도우 메세지 가져옴
		// 나머지는 필터링
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// 가상 키 입력인 경우 문자 입력 WM_CHAR으로 바꿈
			TranslateMessage(&message);
			// 윈도우 프로시저로 메세지 보냄
			DispatchMessage(&message);
		}
		else
		{
			Update();
			//Render
			InvalidateRect(mhwnd, NULL, false);
		}
	}
}

HRESULT GameCore::CreateDeviceSource()
{
	HRESULT hr = S_OK;
	if (!mpMainRenderTarget)
	{
		RECT rc;
		GetClientRect(mhwnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		hr = mpD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(mhwnd, size),
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

void GameCore::DiscardDeviceResources()
{
	SAFE_RELEASE(mpMainRenderTarget);
	SAFE_RELEASE(mpEmptyGrid);
	SAFE_RELEASE(mpTGrid);
	SAFE_RELEASE(mpLGrid);
	SAFE_RELEASE(mpRLGrid);
	SAFE_RELEASE(mpZGrid);
	SAFE_RELEASE(mpRZGrid);
	SAFE_RELEASE(mpOGrid);
	SAFE_RELEASE(mpIGrid);
}

void GameCore::Update()
{
	KI->Update();
	mpGame->Update();
}

void GameCore::Draw()
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
		int(*board)[CST::WIDTH] = mpGame->GetBoard();

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

LRESULT GameCore::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	if (message == WM_CREATE)
	{
		// https://docs.microsoft.com/ko-kr/windows/win32/api/winuser/ns-winuser-createstructa
		// LPCRESTESTRUCT는 CREATESCRUCT의 포인터일 뿐
		// WM_CREATE시 wParam은 사용되지 않고 lParam에 createstruct의 포인터를 전달함
		// https://docs.microsoft.com/ko-kr/windows/win32/learnwin32/managing-application-state-
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		GameCore* pGameCore = (GameCore*)pcs->lpCreateParams;
		// hwnd의 추가메모리에 USERDATA측 pGameCore의 주소를 저장
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pGameCore));

		// 화면의 중앙
		RECT desktopWindow;
		// GetDesktopWindow()함수는 데스크톱 화면의 핸들을 가져온다.
		GetWindowRect(GetDesktopWindow(), &desktopWindow);

		int x = desktopWindow.right / 2;
		int y = desktopWindow.bottom / 2;

		RECT rt = { 0,0,480,640 };
		AdjustWindowRect(&rt, WS_CAPTION | WS_SYSMENU, false);
		MoveWindow(hwnd, x - (rt.right - rt.left) / 2,
			y - (rt.bottom - rt.top) / 2,
			rt.right - rt.left,
			rt.bottom - rt.top,
			true
		);

		result = 1;
	}
	else
	{
		// hwnd의 추가메모리 USERDATA부분에 저장된 값을 가져옴 -> pGameCore의 주소
		GameCore* pGameCore = reinterpret_cast<GameCore*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		bool wasHandled = false;

		if (pGameCore)
		{
			switch (message)
			{
			case WM_DISPLAYCHANGE:
			{
				// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-invalidaterect
				// hwnd의 영역(NULL)을 무효화하여 다시 화면에 그리게 함
				InvalidateRect(hwnd, NULL, FALSE);
			}
			wasHandled = true;
			result = 0;
			break;
			case WM_PAINT:
			{
				pGameCore->Draw();
				// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-validaterect
				ValidateRect(hwnd, NULL);
			}
			wasHandled = true;
			result = 0;
			break;
			case WM_DESTROY:
			{
				pGameCore->isRun = false;
				PostQuitMessage(0);
			}
			wasHandled = true;
			result = 1;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return result;
}

HRESULT GameCore::CreateBitmapGridBlock(ID2D1HwndRenderTarget*& renderTarget, ID2D1BitmapBrush*& gridBitmap, D2D1::ColorF::Enum color, bool isTwoTone)
{

	HRESULT hr = S_OK;

	// 호환 렌더타겟을 생성.
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	hr = renderTarget->CreateCompatibleRenderTarget(D2D1::SizeF(CST::BLOCK_SIZE, CST::BLOCK_SIZE), &pCompatibleRenderTarget);
	if (SUCCEEDED(hr))
	{
		ID2D1SolidColorBrush* gridColor1 = nullptr;
		ID2D1SolidColorBrush* gridColor2 = nullptr;
		D2D1::ColorF color2(DarkenColor(color, 5));
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
