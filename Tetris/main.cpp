#include "GameCore.h"

// _In_ : 매개 변수는 유효 해야 하며 수정 되지 않음, 읽기 전용
// _In_opt :  In와 동일하나 NULL 입력 가능
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// CoInitialize : COM라이브러리 사용을 위한 초기화 (COM라이브라리 : DirectX)
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		GameCore GameCore;

		if (SUCCEEDED(GameCore.Initialize(hInstance)))
		{
			GameCore.RunGameLoop();
		}
		// CoUninitialize : COM라이브러리 상용 후 해체
		CoUninitialize();
	}
	return 0;
}