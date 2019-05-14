#include "Game/Game.h"

#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	Game::Get().Init();
	Game::Get().Update();

	return 0;
}
