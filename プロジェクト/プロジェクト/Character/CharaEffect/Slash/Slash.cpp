#include "Slash.h"
#include "../../Player/Player.h"

// コンストラクタ
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

// デストラクタ
Slash::~Slash()
{
}

// 更新
void Slash::Update()
{
	if (stFunc.find(state) == stFunc.end())
	{
		return;
	}

	stFunc[state]();
}

// 描画
void Slash::Draw()
{
}
void Slash::Draw(std::weak_ptr<MyLib> lib)
{
	AnimationUpdate();

	DrawImage();
}

// 状態初期化
void Slash::InitState()
{
	stFunc[CharacterState::Attack1] = std::bind(&Slash::FirstAttackUpdate, this);
	stFunc[CharacterState::Attack2] = std::bind(&Slash::SecondAttackUpdate, this);
	stFunc[CharacterState::Attack3] = std::bind(&Slash::ThirdAttackUpdate, this);
}

// 初撃
void Slash::FirstAttackUpdate()
{
	AttackUpdate(15);
}

// 二撃
void Slash::SecondAttackUpdate()
{
	AttackUpdate(15);
}

// 三撃
void Slash::ThirdAttackUpdate()
{
	AttackUpdate(25);
}

// 攻撃処理
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
