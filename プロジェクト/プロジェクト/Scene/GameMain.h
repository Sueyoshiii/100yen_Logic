#pragma once
#include "Scene.h"
#include <memory>

class Player;

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
	// �v���C���[
	std::shared_ptr<Player> pl;
};
