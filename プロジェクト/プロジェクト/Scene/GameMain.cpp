#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../Character/Player.h"

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib) :
	t("img/brides.jpg")
{
	this->lib = lib;
	cam = std::make_shared<Camera>();
	pl = std::make_shared<Player>(lib, cam);
	cam->SetFocus(pl);
}

// �f�X�g���N�^
GameMain::~GameMain()
{
}

// �`��
void GameMain::Draw(void)
{
	lib.lock()->Draw(t);
	pl->Draw();
}

// ����
void GameMain::UpData(void)
{
	cam->Update();
	pl->Update();
}