#pragma once
#include "Scene.h"

class Clear :
	public Scene
{
public:
	// �R���X�g���N�^
	Clear(std::weak_ptr<MyLib>lib);
	// �f�X�g���N�^
	~Clear();

	// �`��
	void Draw();

	// ����
	void Update();

private:
};
