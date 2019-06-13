#pragma once
#include <MyLib.h>
#include <memory>
#include <vector>

class Camera;

class BackGround
{
public:
	BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~BackGround();

	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// �w�i�摜
	std::vector<Texture> tex;

	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �J����
	std::weak_ptr<Camera> cam;

	// ���]�t���O
	bool turnFlag;
};