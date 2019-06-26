#include "Slash.h"
#include "../../Player/Player.h"

// �R���X�g���N�^
Slash::Slash(const CharacterState& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag)
{
	this->state = state;

	LoadData("data/chara/player_effect.info");
	LoadImage("img/Player/player_effect.png");

	InitState();

	tex.pos = Vec2f(pos.x + size.x / 2.0f, pos.y);
	tex.size = size;
	this->turnFlag = turnFlag;
}

// �f�X�g���N�^
Slash::~Slash()
{
}

// �X�V
void Slash::Update()
{
	if (stFunc.find(state) == stFunc.end())
	{
		return;
	}

	stFunc[state]();
}

// �`��
void Slash::Draw()
{
}
void Slash::Draw(std::weak_ptr<MyLib> lib)
{
	AnimationUpdate();

	DrawImage();
}

// ��ԏ�����
void Slash::InitState()
{
	stFunc[CharacterState::Attack1] = std::bind(&Slash::FirstAttackUpdate, this);
	stFunc[CharacterState::Attack2] = std::bind(&Slash::SecondAttackUpdate, this);
	stFunc[CharacterState::Attack3] = std::bind(&Slash::ThirdAttackUpdate, this);
}

// ����
void Slash::FirstAttackUpdate()
{
	AttackUpdate(15);
}

// ��
void Slash::SecondAttackUpdate()
{
	AttackUpdate(15);
}

// �O��
void Slash::ThirdAttackUpdate()
{
	AttackUpdate(25);
}

// �U������
void Slash::AttackUpdate(const unsigned int attackInterval)
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		++attackCnt;
	}

	if (attackCnt > attackInterval)
	{
		attackCnt = 0;
		stopFlag = false;
		deleteFlag = true;
	}
}
