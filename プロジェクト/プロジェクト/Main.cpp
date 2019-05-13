#include <MyLib.h>

#pragma comment(lib, "Lib.lib")

#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	MyLib lib(640);
	lib.ChangeTitle("ヒャクエンロジック");

	while (lib.CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{
		lib.Clear();
		lib.Execution();
	}

	return 0;
}
