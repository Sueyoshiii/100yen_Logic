#pragma once
#include "Scene.h"

class Over :
	public Scene
{
public:
	// �R���X�g���N�^
	Over(std::weak_ptr<MyLib>lib);
	// �f�X�g���N�^
	~Over();

	// �`��
	void Draw();

	// ����
	void UpData();

private:

};
