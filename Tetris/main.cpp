#include "GameCore.h"

// _In_ : �Ű� ������ ��ȿ �ؾ� �ϸ� ���� ���� ����, �б� ����
// _In_opt :  In�� �����ϳ� NULL �Է� ����
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// CoInitialize : COM���̺귯�� ����� ���� �ʱ�ȭ (COM���̺�� : DirectX)
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		GameCore GameCore;

		if (SUCCEEDED(GameCore.Initialize(hInstance)))
		{
			GameCore.RunGameLoop();
		}
		// CoUninitialize : COM���̺귯�� ��� �� ��ü
		CoUninitialize();
	}
	return 0;
}