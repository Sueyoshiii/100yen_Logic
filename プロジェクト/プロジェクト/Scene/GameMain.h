#pragma once
#include "Scene.h"
#include <memory>
#include <MyLib.h>

class Camera;
class BackGround;
class Player;
class Wolf;

class GameMain :
	public Scene
{
public:
	// �R���X�g���N�^
	GameMain(std::weak_ptr<MyLib>lib);
	// �f�X�g���N�^
	~GameMain();

	// �`��
	void Draw(void);
	
	// ����
	void UpData(void);

private:
	// �J����
	std::shared_ptr<Camera> cam;

	// �w�i
	std::shared_ptr<BackGround> bg;

	// �v���C���[
	std::shared_ptr<Player> pl;

	// �T
	// �ŏI�I�ɂ͓G�͈ꊇ�Ǘ�
	std::shared_ptr<Wolf> wolf;
};
