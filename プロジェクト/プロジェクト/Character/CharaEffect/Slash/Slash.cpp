#include "Slash.h"

// �R���X�g���N�^
Slash::Slash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag) :
	attackCnt(0)
{
	InitState();

	this->state = stateMap[state];

	LoadData("data/chara/player_effect.info");
	LoadImage("img/Player/player_effect.png");

	ChangeState(this->state);

	this->turnFlag = turnFlag;
	float x  = this->turnFlag ? pos.x - size.x / 1.5f : pos.x + size.x / 1.5f;
	tex.pos  = Vec2f(x, pos.y);
	tex.size = size;
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
	AnimationUpdate();

	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

// �폜
void Slash::Delete()
{
	tex.Delete("img/Player/player_effect.png");
}

// ��ԏ�����
void Slash::InitState()
{
	stateMap["Attack1"] = "first";
	stateMap["Attack2"] = "second";
	stateMap["Attack3"] = "third";

	func.clear();
	func["first"]  = std::bind(&Slash::FirstAttackUpdate, this);
	func["second"] = std::bind(&Slash::SecondAttackUpdate, this);
	func["third"]  = std::bind(&Slash::ThirdAttackUpdate, this);
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
