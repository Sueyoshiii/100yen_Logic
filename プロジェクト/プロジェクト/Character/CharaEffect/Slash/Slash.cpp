#include "Slash.h"

// コンストラクタ
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

// 状態初期化
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

// 四撃
void Slash::FourthAttackUpdate()
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

// ジャンプ攻撃
void Slash::JumpAttackUpdate()
{
	if (CheckAnimEnd())
	{
		deleteFlag = true;
	}
}

// 座標設定
void Slash::SetPos(const Vec2f& pos)
{
	float x = this->turnFlag ? pos.x - tex[type].size.x / 2.0f : pos.x + tex[type].size.x / 2.0f;
	tex[type].pos = Vec2f(x, pos.y);
}
