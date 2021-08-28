#include <cassert>

#include "GameCore.h"
#include "GameBoard.h"
#include "CST.h"
#include "Macro.h"
#include "KeyInput.h"
#include "GameBoardScene.h"
#include "MainScene.h"
#include "SceneManager.h"
#include "Factorys.h"
GameCore::GameCore()
	: mhwnd(nullptr)
	, isRun(true)
{
}

GameCore::~GameCore()
{
	Factorys::DeleteInstance();
}

HRESULT GameCore::Initialize(HINSTANCE hInstance)
{
	HRESULT hr;
	// 창 생성
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
	// 최초 씬 생성
	SM->SetScene(
		new MainScene(mhwnd, hInstance)
	);
	// 창 보이기
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
			KI->Update();
			SM->GetScene()->Update();
			//Render
			InvalidateRect(mhwnd, NULL, false);
		}
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
				SM->GetScene()->Render();
				// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-validaterect
				ValidateRect(hwnd, NULL);
			}
			wasHandled = true;
			result = 0;
			break;
			case WM_LBUTTONUP:
				POINT pt;
				GetCursorPos(&pt);

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