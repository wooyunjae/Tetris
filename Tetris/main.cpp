#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include "GameCore.h"
#include "Factorys.h"

// _In_ : �Ű� ������ ��ȿ �ؾ� �ϸ� ���� ���� ����, �б� ����
// _In_opt :  In�� �����ϳ� NULL �Է� ����
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

	// CoInitialize : COM���̺귯�� ����� ���� �ʱ�ȭ (COM���̺�� : DirectX)
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{// scope ����
			Factorys::GetInstance()->Init();
			GameCore pGameCore;

			if (SUCCEEDED(pGameCore.Initialize(hInstance)))
			{
				pGameCore.RunGameLoop();
			}
			Factorys::GetInstance()->DeleteInstance();
		}// scope ������ GameCore �Ҹ�
		// CoUninitialize : COM���̺귯�� ��� �� ��ü
		CoUninitialize();
	}
#ifdef _DEBUG
	FreeConsole();
#endif
	return 0;
}