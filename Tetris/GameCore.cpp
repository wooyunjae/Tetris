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
	// â ����
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
	// ���� �� ����
	SM->SetScene(
		new MainScene(mhwnd, hInstance)
	);
	// â ���̱�
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
		// �޼��� ť�� ������� �޼��� ����
		// NULL : ���� �������� ��� ������ �޼��� ������
		// �������� ���͸�
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			// ���� Ű �Է��� ��� ���� �Է� WM_CHAR���� �ٲ�
			TranslateMessage(&message);
			// ������ ���ν����� �޼��� ����
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
		// LPCRESTESTRUCT�� CREATESCRUCT�� �������� ��
		// WM_CREATE�� wParam�� ������ �ʰ� lParam�� createstruct�� �����͸� ������
		// https://docs.microsoft.com/ko-kr/windows/win32/learnwin32/managing-application-state-
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		GameCore* pGameCore = (GameCore*)pcs->lpCreateParams;
		// hwnd�� �߰��޸𸮿� USERDATA�� pGameCore�� �ּҸ� ����
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pGameCore));
		result = 1;
	}
	else
	{
		// hwnd�� �߰��޸� USERDATA�κп� ����� ���� ������ -> pGameCore�� �ּ�
		GameCore* pGameCore = reinterpret_cast<GameCore*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));

		bool wasHandled = false;

		if (pGameCore)
		{
			switch (message)
			{
			case WM_DISPLAYCHANGE:
			{
				// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-invalidaterect
				// hwnd�� ����(NULL)�� ��ȿȭ�Ͽ� �ٽ� ȭ�鿡 �׸��� ��
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