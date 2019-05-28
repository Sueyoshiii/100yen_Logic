#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Character/Player.h"
#include "../Character/EnemyManager.h"
#include "../Character/Wolf.h"

#define Enem EnemyManager::Get()

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	cam = std::make_shared<Camera>();
	bg = std::make_shared <BackGround>(lib, cam);
	pl = std::make_shared<Player>(lib, cam);
	cam->SetFocus(pl);

	// �G����B
	Enem.Summons(EM::Wolf, Vec2f(60.0f, 0.0f), lib, pl, cam);
	Enem.Summons(EM::Wolf, Vec2f(260.0f, 0.0f), lib, pl, cam);
}

// �f�X�g���N�^
GameMain::~GameMain()
{
}

// �`��
void GameMain::Draw(void)
{
	bg->Draw();
	pl->Draw();
	Enem.Draw();
}

// ����
void GameMain::UpData(void)
{
	cam->Update();
	bg->Update();
	pl->Update();
	Enem.Update();
}