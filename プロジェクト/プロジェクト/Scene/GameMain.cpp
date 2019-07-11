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
	stage.reset(new FirstRoom(lib, pl, cam));

	// �Ώۂ��v���C���[�ɂ���
	cam->SetFocus(pl);

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
	}

	// �v���C���[
	pl->Draw();

	if (pl->GetState() != "Death")
	{
		// �G�t�F�N�g
		EffectManager::Get().Draw(lib);
	}

	// �J�ڗp�{�b�N�X�`��
	stage->DrawBox();
}

// ����
void GameMain::UpData()
{
	if (!stage->GetNextRoomFlag())
	{
		// �J����
		cam->Update();

		// �w�i
		bg->Update();

		// ���[��
		stage->Update();

		// �v���C���[
		pl->Update();

		// �G�t�F�N�g
		EffectManager::Get().Update();
	}

	// ���[���؂�ւ��`�F�b�N
	CheckChangeRoom();

	// �N���A���̓N���A�V�[����
	if (INPUT.IsTrigger(Key::P))
	{
		ChangeNextScene(new Clear(lib));
	}
	// ���S���̓Q�[���I�[�o�[�V�[����
	else if (pl->GetDeadEndFlag())
	{
		ChangeNextScene(new Over(lib));
	}
}

// �V�[���؂�ւ�
void GameMain::ChangeNextScene(Scene* scene)
{
	// �o�^����Ă���I�u�W�F�N�g�̉��
	DeleteObject();

	// �V�[���؂�ւ�
	Game::Get().ChangeScene(scene);
}

// ���[���؂�ւ�
void GameMain::CheckChangeRoom()
{
	if (pl->GetWorldPos().x + pl->GetSize().x / 2.0f > StageManager::Get().GetRange().Right())
	{
		stage->SetNextRoomFlag(true);
		if (stage->GetBoxAlpha() >= 1.0f)
		{
			DeleteObject();
			pl->SetPos(pl->GetFirstPos());
			cam->SetPos(pl->GetWorldPos());
			stage.reset(stage->GetNextRoom());
		}
	}
}

// �o�^����Ă���I�u�W�F�N�g�̉��
void GameMain::DeleteObject()
{
	EnemyManager::Get().Delete();
	EffectManager::Get().Delete();
}
