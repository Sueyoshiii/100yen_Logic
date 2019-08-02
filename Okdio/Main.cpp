#include "Okdio/Okdio.h"
#include <vector>
#include <Windows.h>

int main()
{
	auto hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	_ASSERT(hr == S_OK);

	okmonn::EnginStart();

	Okdio* okdio = nullptr;
	hr = okmonn::CreateObj(IID_PPV_ARGS(&okdio));
	if (FAILED(hr))
	{
		return 0;
	}

	okdio->Load("INSIDE.wav");
	okdio->Play(true);

	float volume = 1.0f;
	const float tmp = 0.00001f;
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{
		if (GetKeyState(VK_UP) & 0x80)
		{
			if (okdio->SetVolume(volume + tmp))
			{
				volume += tmp;
			}
		}
		else if (GetKeyState(VK_DOWN) & 0x80)
		{
			if (okdio->SetVolume(volume - tmp))
			{
				volume -= tmp;
			}
		}
	}

	okdio->Release();
	okmonn::EnginEnd();
	CoUninitialize();
	return 0;
}
