#include "SoundManager.h"
#include "../../Okdio/Okdio/Okdio.h"
#pragma comment (lib, "Okdio.lib")

SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}


void SoundManager::Create(void)
{
	for (auto& i : sound)
	{
		okmonn::CreateObj(IID_PPV_ARGS(&i));
	}

	sound[0]->Load("data/sound/bgm/bgm_stage.wav");
	sound[1]->Load("data/sound/bgm/bgm_boss.wav");
}

long SoundManager::Play(const unsigned int& index)
{
	if (index >= sound.size())
	{
		return S_FALSE;
	}

	return sound[index]->Play(true);
}

long SoundManager::Stop(const unsigned int& index)
{
	if (index >= sound.size())
	{
		return S_FALSE;
	}

	return sound[index]->Stop();
}

void SoundManager::Release(void)
{
	for (auto& i : sound)
	{
		if (i != nullptr)
		{
			i->Stop();
			i->Release();
			i = nullptr;
		}
	}
}

