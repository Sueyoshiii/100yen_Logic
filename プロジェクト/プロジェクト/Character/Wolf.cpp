#include "Wolf.h"

// コンストラクタ
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/enemy_1.info");
	LoadImage("img/Enemy_1.png");

	InitFunc();
	ChangeState(ST::Neutral);

	speed   = 4.0f;
	dushPow = 10.0f;
	jumpPow = -18.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

	turnFlag = true;

	localPos = pos;
}

// デストラクタ
Wolf::~Wolf()
{
}

// 更新
void Wolf::Update()
{
	func[state]();

	FallUpdate();

	UpdateLocalPos();

	CheckHit();
}

// 描画
void Wolf::Draw()
{
	AnimationUpdate();

	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

// 状態初期化
void Wolf::InitFunc()
{
	func.clear();
	
	func[ST::Neutral] = std::bind(&Wolf::NeutralUpdate, this);
	func[ST::Walk]    = std::bind(&Wolf::WalkUpdate, this);
}

// 待機
void Wolf::NeutralUpdate()
{
}

// 歩き
void Wolf::WalkUpdate()
{
}
