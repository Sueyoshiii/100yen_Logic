#include "Slash.h"

// コンストラクタ
Slash::Slash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag) :
	attackCnt(0)
{
	InitState();

	this->turnFlag = turnFlag;
	this->state = stateMap[state];
	LoadData("data/chara/player_effect.info");
	LoadImage("img/Player/player_effect.png");
	ChangeState(this->state);
	tex.pos = Vec2f(pos.x + size.x / 2.0f, pos.y);
	tex.size = size;
}

// デストラクタ
Slash::~Slash()
{
}

// 更新
void Slash::Update()
{
	func[this->state]();
}

// 描画
void Slash::Draw()
{
}
void Slash::Draw(std::weak_ptr<MyLib> lib)
{
	if (deleteFlag)
	{
		return;
	}
	AnimationUpdate();

	DrawImage();
}

// 状態初期化
void Slash::InitState()
{
	stateMap["Attack1"] = "first";
	stateMap["Attack2"] = "second";
	stateMap["Attack3"] = "third";

	func["first"]  = std::bind(&Slash::FirstAttackUpdate, this);
	func["second"] = std::bind(&Slash::SecondAttackUpdate, this);
	func["third"]  = std::bind(&Slash::ThirdAttackUpdate, this);
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
