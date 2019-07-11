#pragma once
#include "Scene.h"
#include <memory>

class Camera;
class BackGround;
class Player;
class Stage;

class GameMain :
	public Scene
{
public:
	// �R���X�g���N�^
	GameMain(std::weak_ptr<MyLib>lib);
	// �f�X�g���N�^
	~GameMain();

	// �`��
	void Draw();
	
	// ����
	void UpData();
private:
	// �V�[���؂�ւ�
	void ChangeNextScene(Scene* scene);

	// ���[���؂�ւ�
	void ChangeRoom();

	// �o�^����Ă���I�u�W�F�N�g�̉��
	void DeleteObject();

	// �J����
	std::shared_ptr<Camera> cam;

	// �w�i
	std::shared_ptr<BackGround> bg;

	// �v���C���[
	std::shared_ptr<Player> pl;

	// �X�e�[�W
	std::shared_ptr<Stage> stage;
};
