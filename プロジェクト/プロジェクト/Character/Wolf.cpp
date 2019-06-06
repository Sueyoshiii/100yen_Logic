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
	ChangeState(ST::Walk);

	tex.size *= 2.0f;

	speed   = 2.0f;
	dushPow = 10.0f;
	jumpPow = -30.0f;
	vel     = Vec2f(speed, 0.0f);

	hp = 2;

	turnFlag = false;

	tex.pos = pos;
	worldPos = cam.lock()->Correction(tex.pos);
	fulcrum = worldPos;

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

	//CheckHit();
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
	static unsigned int cnt = 0;
	if (oldState == ST::Walk || oldState == ST::Attack1)
	{
		if (!CheckAnimEnd())
		{
			return;
		}

		if (cnt >= 2)
		{
			cnt = 0;
			turnFlag = !turnFlag;
			vel.x = -vel.x;
			Walk();
		}
		++cnt;
	}
}
// �p�j
void Wolf::Loitering()
{
	worldPos.x += vel.x;

	float size = tex.size.x * 2.0f;
	if (fulcrum.x - size >= worldPos.x ||
		worldPos.x >= fulcrum.x + size)
	{
		oldState = state;
		ChangeState(ST::Neutral);
	}
}
// ����
// �U���͈͂܂ňړ�
// ���������ۂ̃A�N�V�����ق���
// ���W�����v����Ƃ�
void Wolf::Alert()
{
	static unsigned int cnt = 0;
	if ((++cnt) % 10 == 0)
	{
		if (worldPos.x > pl.lock()->GetWorldPos().x)
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

	float dis = fabs(worldPos.x - pl.lock()->GetWorldPos().x);
	if (0.0f < dis && dis < 150)
	{
		Attack();
	}
	else
	{
		worldPos.x += vel.x;
	}
}

// ����
void Wolf::WalkUpdate()
{
	// ���E�ɑ�����܂Ŝp�j
	static bool discovery = false;
	if (CheckView())
	{
		discovery = true;
	}

	if (discovery)
	{
		Alert();
	}
	else
	{
		Loitering();
	}
}
void Wolf::Walk()
{
	ChangeState(ST::Walk);
}

// �U��
void Wolf::AttackUpdate()
{
	if (CheckAnimEnd())
	{
		stopFlag = true;
		if ((++coolTime) > 60)
		{
			stopFlag = false;
			ChangeState(ST::Neutral);
		}
	}
	else
	{
		vel.x = worldPos.x > pl.lock()->GetWorldPos().x ? -speed*2 : speed*2;
		worldPos.x += vel.x;
	}

	/*
	
	��������
		���x�FvX = v0cos��
		�ψʁFx  = v0cos��*t
	��������
		���x�FvY = v0sin��-gt
		�ψʁFy  = v0sin��*t-1/2gt^2

	*/
}
void Wolf::Attack()
{
	coolTime = 0;
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
