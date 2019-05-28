#include "Wolf.h"

// �R���X�g���N�^
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/enemy_1.info");
	LoadImage("img/Enemy_1.png");

	InitFunc();
	ChangeState(ST::Neutral);

	vel = Vec2f(Const::SPEED, 0.0f);

	turnFlag = true;

	localPos = pos;
}

// �f�X�g���N�^
Wolf::~Wolf()
{
}

// �X�V
void Wolf::Update()
{
	func[state]();

	FallUpdate();

	UpdateLocalPos();

	CheckHit();
}

// �`��
void Wolf::Draw()
{
	AnimationUpdate();

	DrawImage();

//#ifdef _DEBUG
//	DrawRect();
//#endif
}

// ��ԏ�����
void Wolf::InitFunc()
{
	func.clear();
	
	func[ST::Neutral] = std::bind(&Wolf::NeutralUpdate, this);
	func[ST::Walk]    = std::bind(&Wolf::WalkUpdate, this);
}

// �ҋ@
void Wolf::NeutralUpdate()
{
}

// ����
void Wolf::WalkUpdate()
{
}
