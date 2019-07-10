#include "GameMain.h"
#include "../Camera/Camera.h"
#include "../BackGround/BackGround.h"
#include "../Stage/StageManager.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/CharaEffect/EffectManager.h"
#include "../Game/Game.h"
#include "Clear.h"
#include "Over.h"
#include "../Stage/Stage.h"
#include "../Stage/FirstRoom/FirstRoom.h"
#include "../Stage/SecondRoom/SecondRoom.h"

#include "../Json/JsonLoader.h"

// �R���X�g���N�^
GameMain::GameMain(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	// �J�ڗp�{�b�N�X�̐ݒ�
	box = Primitive(PrimitiveType::box);
	box.pos[0] = Vec3f();
	box.pos[1] = Vec3f(float(lib.lock()->GetWinSize().x), 0.0f, 0.0f);
	box.pos[2] = Vec3f(0.0f, float(lib.lock()->GetWinSize().y*2), 0.0f);
	box.pos[3] = Vec3f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y*2), 0.0f);
	boxAlpha   = 1.0f;

	// ����
	cam = std::make_shared<Camera>(lib);
	bg  = std::make_shared <BackGround>(lib, cam);
	pl  = std::make_shared<Player>(lib, cam);

	// �X�e�[�W�f�[�^�̓ǂݍ���
	StageManager::Get().SetRange(lib.lock()->GetWinSize());
	stage.reset(new FirstRoom(lib, cam));

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
		stage->Draw();

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

	stage->DrawBox();
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

	// ���S���̓Q�[���I�[�o�[��
	if (pl->GetDeadEndFlag())
	{
		ChangeNextScene(new Over(lib));
	}

	if (INPUT.IsTrigger(Key::Q))
	{
		pl->SetPos(pl->GetFirstPos());
		stage.reset(new SecondRoom(lib, cam));
	}
	if (INPUT.IsTrigger(Key::W))
	{
		pl->SetPos(pl->GetFirstPos());
		stage.reset(new FirstRoom(lib, cam));
	}
}

// �V�[���؂�ւ�
void GameMain::ChangeNextScene(Scene* scene)
{
	// �o�^����Ă���I�u�W�F�N�g�̉��
	EnemyManager::Get().Delete();
	EffectManager::Get().Delete();

	// �V�[���؂�ւ�
	Game::Get().ChangeScene(scene);
}
