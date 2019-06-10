#pragma once
#include <memory>

class MyLib;

class Scene
{
public:
	// �R���X�g���N�^
	Scene() {}
	// �f�X�g���N�^
	~Scene() {}

	// �`��
	virtual void Draw() = 0;

	// ����
	virtual void UpData() = 0;

protected:
	// ���C�u����
	std::weak_ptr<MyLib>lib;
};
