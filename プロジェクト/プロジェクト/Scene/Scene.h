#pragma once
#include <MyLib.h>
#include <memory>
#include <iostream>

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

	// �摜
	Texture tex;

	// �A���t�@�l
	float alpha;
};
