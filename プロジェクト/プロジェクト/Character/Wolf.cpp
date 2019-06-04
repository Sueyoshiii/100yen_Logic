#include "Wolf.h"

// �R���X�g���N�^
Wolf::Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos)
{
	this->lib = lib;
	this->pl  = pl;
	this->cam = cam;

	LoadData("data/chara/enemy_1.info");
	LoadImage("img/Enemy_1.png");

	InitFunc();
	ChangeState(ST::Neutral);

	tex.size *= 2.0f;

	speed   = 2.0f;
	dushPow = 10.0f;
	jumpPow = -18.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

	turnFlag = true;

	tex.pos = pos;
	worldPos = cam.lock()->Correction(tex.pos);

	knockBackRange = 4.0f;

	viewRange = Vec2f(200.0f, 80.0f);
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

#ifdef _DEBUG
	DrawRect();
	DrawViewRange();
#endif
}

// �ҋ@
void Wolf::NeutralUpdate()
{
	// ���E���ɑ�������ړ�
	if (CheckView())
	{
		Walk();
	}
}

// ����
void Wolf::WalkUpdate()
{
	static unsigned int cnt = 0;
	if ((++cnt) % 50 == 0)
	{
		if (pl.lock()->GetPos().x < tex.pos.x)
		{
			turnFlag = true;
			vel.x = -speed;
		}
		else
		{
			turnFlag = false;
			vel.x = speed;
		}
	}
	worldPos.x += vel.x;

	static unsigned int trackingCnt = 0;
	if (++trackingCnt % (60 * 2) == 0)
	{
		// ���b���Ɉ�x�A�ǐՌp�����ۂ����f
		if (!CheckView())
		{
			ChangeState(ST::Neutral);
		}
	}
}
void Wolf::Walk()
{
	ChangeState(ST::Walk);
}

// �U��
void Wolf::AttackUpdate()
{
	// �ڕW�n�_�փW�����v�U���������[
	static unsigned int cnt = 0;
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if ((++cnt) > 10)
		{
			cnt = 0;
			stopFlag = false;
			ChangeState(ST::Neutral);
		}
	}
}
void Wolf::Attack()
{
	vel.y = jumpPow;
	oldPlPos = pl.lock()->GetWorldPos();
	ChangeState(ST::Attack1);
}

// ��_���[�W
void Wolf::DamageUpdate()
{
	static unsigned int cnt = 0;
	if (worldPos.y < Const::GROUND)
	{
		worldPos.x += vel.x;
	}
	else
	{
		if ((++cnt) > 40)
		{
			cnt = 0;
			if (hp >= 0)
			{
				ChangeState(ST::Neutral);
			}
			else
			{
				ChangeState(ST::Death);
			}
		}
	}
}

// ���S
void Wolf::DeathUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if (In.IsTrigger(Key::S))
		{
			stopFlag = false;
			hp = 3;
			ChangeState(ST::Neutral);
		}
	}
}

// ��ԏ�����
void Wolf::InitFunc()
{
	func.clear();

	func[ST::Neutral] = std::bind(&Wolf::NeutralUpdate, this);
	func[ST::Walk]    = std::bind(&Wolf::WalkUpdate, this);
	func[ST::Attack1] = std::bind(&Wolf::AttackUpdate, this);
	func[ST::Damage]  = std::bind(&Wolf::DamageUpdate, this);
	func[ST::Death]   = std::bind(&Wolf::DeathUpdate, this);
}
