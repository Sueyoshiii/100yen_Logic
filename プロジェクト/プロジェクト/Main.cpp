#include "Game/Game.h"
#include <Windows.h>
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
#endif
{
	Game::Get().Init();
	Game::Get().Update();

	Game::Get().Finish();

	return 0;
}
