#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Stage/Stage.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/CharaEffect/EffectManager.h"

#include "../Json/JsonLoader.h"

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	cam = std::make_shared<Camera>(lib);
	bg  = std::make_shared <BackGround>(lib, cam);
	pl  = std::make_shared<Player>(lib, cam);

	// �X�e�[�W�f�[�^�̓ǂݍ���
	Stage::Get().Load(cam, "data/stage/map.json", "img/Stage/tileset.png");

	// �Ώۂ��v���C���[�ɂ���
	cam->SetFocus(pl);
	
	// �G����B
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(100.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(600.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(800.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(1000.0f, 0.0f), lib, pl, cam);
	EnemyManager::Get().Summons(Enemies::Wolf, Vec2f(1200.0f, 0.0f), lib, pl, cam);

	// test
	//JsonLoader::Get().Load("data/stage/map.json");

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
	if (pl->GetState() != "Death")
	{
		// �w�i
		bg->Draw();

		// �X�e�[�W
		Stage::Get().Draw(lib);

		// �G
		EnemyManager::Get().Draw();
	}

	// �v���C���[
	pl->Draw();

	if (pl->GetState() != "Death")
	{
		// �G�t�F�N�g
		EffectManager::Get().Draw(lib);
	}
}

// ����
void GameMain::UpData()
{
	// �J����
	cam->Update();

	// �w�i
	bg->Update();

	// �G
	EnemyManager::Get().Update();

	// �v���C���[
	pl->Update();

	// �G�t�F�N�g
	EffectManager::Get().Update();
}