#include "GameMain.h"
#include "../Character/Player.h"
#include <MyLib.h>

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
	pl = std::make_shared<Player>(lib);
}

// �f�X�g���N�^
GameMain::~GameMain()
{
}

// �`��
void GameMain::Draw(void)
{
	pl->Draw();
}

// ����
void GameMain::UpData(void)
{
	pl->Update();
}