#pragma once
#include <array>

class Okdio;

class SoundManager
{
public:
	~SoundManager();
	static SoundManager& Get() { static SoundManager instance; return instance; }

	void Create(void);

	// �Đ�
	long Play(const unsigned int& index);

	// ��~
	long Stop(const unsigned int& index);

	// �T�E���h���
	void Release(void);
private:
	SoundManager();
	SoundManager(const SoundManager&){}
	void operator=(const SoundManager&){}

	std::array<Okdio*, 2>sound;
};

