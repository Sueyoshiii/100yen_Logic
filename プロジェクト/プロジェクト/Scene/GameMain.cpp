#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Stage/Stage.h"
#include "../Character/Player.h"
#include "../Character/EnemyManager.h"

#define Enemy EnemyManager::Get()

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	cam = std::make_shared<Camera>(lib);
	bg = std::make_shared <BackGround>(lib, cam);
	pl = std::make_shared<Player>(lib, cam);

	// �X�e�[�W�f�[�^�̓ǂݍ���
	Stage::Get().Load("data/stage/untitled.json");

	// �Ώۂ��v���C���[�ɂ���
	cam->SetFocus(pl);

	// �G����B
	Enemy.Summons(EM::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(800.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(1000.0f, 0.0f), lib, pl, cam);
	Enemy.Summons(EM::Wolf, Vec2f(1200.0f, 0.0f), lib, pl, cam);

#ifdef _DEBUG
	std::cout << "GameMain Scene" << std::endl;
#endif
}

// �f�X�g���N�^
GameMain::~GameMain()
{
}

// �`��
void GameMain::Draw()
{
	// �w�i
	bg->Draw();

	// �X�e�[�W
	Stage::Get().Draw();

	// �G
	Enemy.Draw();

	// �v���C���[
	pl->Draw();
}

// ����
void GameMain::UpData()
{
	cam->Update();
	bg->Update();
	Enemy.Update();
	pl->Update();
}