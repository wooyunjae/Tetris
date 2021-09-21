#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include "GameCore.h"
#include "Factorys.h"

// _In_ : 매개 변수는 유효 해야 하며 수정 되지 않음, 읽기 전용
// _In_opt :  In와 동일하나 NULL 입력 가능
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	if (AllocConsole())
	{
#pragma warning (disable : 6031)
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
#pragma warning (default : 6031)
	}
#endif

	// CoInitialize : COM라이브러리 사용을 위한 초기화 (COM라이브라리 : DirectX)
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{// scope 시작
			Factorys::GetInstance()->Init();
			GameCore pGameCore;

			if (SUCCEEDED(pGameCore.Initialize(hInstance)))
			{
				pGameCore.RunGameLoop();
			}
			Factorys::GetInstance()->DeleteInstance();
		}// scope 끝으로 GameCore 소멸
		// CoUninitialize : COM라이브러리 상용 후 해체
		CoUninitialize();
	}
#ifdef _DEBUG
	FreeConsole();
#endif
	return 0;
}