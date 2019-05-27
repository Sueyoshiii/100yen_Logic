#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Character/Player.h"
#include "../Character/Wolf.h"

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	cam = std::make_shared<Camera>();
	bg = std::make_shared <BackGround>(lib, cam);
	pl = std::make_shared<Player>(lib, cam);
	wolf = std::make_shared<Wolf>(lib, pl, cam);
	cam->SetFocus(pl);
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
	wolf->Draw();
}

// ����
void GameMain::UpData(void)
{
	cam->Update();
	bg->Update();
	pl->Update();
	wolf->Update();
}