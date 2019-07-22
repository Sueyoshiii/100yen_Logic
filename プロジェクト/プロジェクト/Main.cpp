#include "Game/Game.h"
//#include <Okdio.h>

#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
#endif
{
	//auto hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	//_ASSERT(hr == S_OK);

	Game::Get().Init();
	Game::Get().Update();

	Game::Get().Finish();

	//CoUninitialize();
	return 0;
}
