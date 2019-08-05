#include "Slash.h"

// �R���X�g���N�^
Slash::Slash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag) :
	attackCnt(0)
{
	InitState();

	this->state = stateMap[state];

	InitType();
	typeFunc[plType]();

	ChangeState(this->state);

	this->turnFlag = turnFlag;
	float x = this->turnFlag ? pos.x - size.x / 2.0f : pos.x + size.x / 2.0f;
	tex[type].pos  = Vec2f(x, pos.y);
	tex[type].size = size;
}

// �f�X�g���N�^
Slash::~Slash()
{
}

// �X�V
void Slash::Update()
{
	func[this->state]();
}

// �`��
void Slash::Draw()
{
}
void Slash::Draw(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	if (deleteFlag) 
	{
		return;
	}

	AnimationUpdate();
	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

// ��ԏ�����
void Slash::InitState()
{
	stateMap["Attack1"]    = "first";
	stateMap["Attack2"]    = "second";
	stateMap["Attack3"]    = "third";
	stateMap["Attack4"]    = "fourth";
	stateMap["JumpAttack"] = "jattack";

	func.clear();
	func["first"]   = std::bind(&Slash::FirstAttackUpdate, this);
	func["second"]  = std::bind(&Slash::SecondAttackUpdate, this);
	func["third"]   = std::bind(&Slash::ThirdAttackUpdate, this);
	func["fourth"]  = std::bind(&Slash::FourthAttackUpdate, this);
	func["jattack"] = std::bind(&Slash::JumpAttackUpdate, this);
}

void Slash::InitType()
{
	typeFunc.clear();
	typeFunc[CharacterType::PL_NORMAL] = std::bind(&Slash::TypeNormal, this);
	typeFunc[CharacterType::PL_WOLF] = std::bind(&Slash::TypeWolf, this);
}

void Slash::TypeNormal()
{
	type = CharacterType::EF_PL_NORMAL_SLASH;
	LoadData("data/chara/player_effect.info");
	LoadImg("img/Player/player_effect.png");
}

void Slash::TypeWolf()
{
	type = CharacterType::EF_PL_WOLF_SLASH;
	LoadData("data/chara/player_wolf_effect.info");
	LoadImg("img/Player/player_wolf_effect.png");
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

// �l��
void Slash::FourthAttackUpdate()
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

// �W�����v�U��
void Slash::JumpAttackUpdate()
{
	if (CheckAnimEnd())
	{
		deleteFlag = true;
	}
}

// ���W�ݒ�
void Slash::SetPos(const Vec2f& pos)
{
	float x = this->turnFlag ? pos.x - tex[type].size.x / 2.0f : pos.x + tex[type].size.x / 2.0f;
	tex[type].pos = Vec2f(x, pos.y);
}
