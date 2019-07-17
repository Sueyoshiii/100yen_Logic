#pragma once
#include "Scene.h"
#include <MyLib.h>
#include <wrl.h>

class Okdio;

class Title :
	public Scene
{
public:
	// �R���X�g���N�^
	Title(std::weak_ptr<MyLib>lib);
	// �f�X�g���N�^
	~Title();

	// �`��
	void Draw();

	// ����
	void Update();
private:
	bool checkNext;
	Okdio* okdio;
};
