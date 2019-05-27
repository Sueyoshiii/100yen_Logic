#include "Wolf.h"

Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam)
{
	this->lib = lib;
	this->pl = pl;
	this->cam = cam;

	LoadData("data/enemy_1.info");
	LoadImage("img/Enemy_1.png");

	InitFunc();
	ChangeState(ST::Neutral);

	vel = Vec2f(Const::SPEED, 0.0f);

	turnFlag = true;

	localPos = Vec2f(100, 0);
}

Wolf::~Wolf()
{
}

void Wolf::Update()
{
	func[state]();

	FallUpdate();

	tex.pos = cam.lock()->Correction(localPos);

	CheckHit();
}

void Wolf::Draw()
{
	AnimationUpdate();

	DrawImage();

#ifdef _DEBUG
	DrawRect();
#endif
}

void Wolf::InitFunc()
{
	func.clear();
	
	func[ST::Neutral] = std::bind(&Wolf::NeutralUpdate, this);
}

void Wolf::NeutralUpdate()
{
}
