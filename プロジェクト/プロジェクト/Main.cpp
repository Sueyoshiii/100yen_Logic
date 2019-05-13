#include <MyLib.h>
#include <iostream>

#pragma comment(lib, "Lib.lib")

#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	MyLib lib(640);
	lib.ChangeTitle("ヒャクエンロジック");
	Texture img("img/nino8.jpg");

	while (lib.CheckMsg() && !(Input::Get().IsKey(Key::Escape)))
	{
		lib.Clear();
		lib.Draw(img);
		lib.Execution();

		if (Input::Get().IsKey(Key::Num4))
		{
			img.pos.x--;
		}
		if (Input::Get().IsKey(Key::Num6))
		{
			img.pos.x++;
		}
		if (Input::Get().IsKey(Key::Num8))
		{
			img.pos.y--;
		}
		if (Input::Get().IsKey(Key::Num2))
		{
			img.pos.y++;
		}
		std::cout << img.pos.x << ", " << img.pos.y << std::endl;
	}

	return 0;
}
