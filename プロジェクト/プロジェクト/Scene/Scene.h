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
	virtual void Update() = 0;

protected:
	// ���C�u����
	std::weak_ptr<MyLib>lib;

	// �摜
	Texture tex;

	// �摜�̃A���t�@�l
	float alpha;

	// �J�ڗp�{�b�N�X
	Primitive box;

	// �J�ڗp�{�b�N�X�̃A���t�@�l
	float boxAlpha;

	// �J�ڊm�F�t���O
	bool checkNext;
};
