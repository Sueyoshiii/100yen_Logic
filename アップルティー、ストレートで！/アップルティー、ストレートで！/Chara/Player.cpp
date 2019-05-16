#include "Player.h"
#include "../Camera/Camera.h"
#include "../Input/Input.h"
#include "../Application/Application.h"

// コンストラクタ
Player::Player(std::weak_ptr<Application>app, std::weak_ptr<Camera>cam, const Vec2f & pos, const Vec2f & size)
{
	this->app  = app;
	this->cam  = cam;
	this->pos  = pos;
	this->lpos = pos;
	this->size = size;

	hp = 10;
	
	LoadImg("pl", "_rsc/img/player.png");
	LoadInfo("_rsc/info/player.info");
	SetFunc();
}

// デストラクタ
Player::~Player()
{
	Delete();
}

// ステータス毎の処理のセット
void Player::SetFunc(void)
{
	func.clear();

	func["wait"]    = &Player::Wait;
	func["walk"]    = &Player::Walk;
	func["sliding"] = &Player::Sliding;
	func["attack1"] = &Player::Attack1;
	func["attack2"] = &Player::Attack2;
	func["avoid"]   = &Player::Avoid;
	func["damage"]  = &Player::Damage;
}

// 待機時の処理
void Player::Wait(void)
{
	if (state != "wait")
	{
		return;
	}

	if (Input::Get().InputKey(INPUT_Z))
	{
		SetState("attack1");
	}
	if (Input::Get().Triger(INPUT_X))
	{
		SetState("sliding");
	}

	if (Input::Get().InputKey(INPUT_LEFT))
	{
		reverse = true;
		SetState("walk");
	}

	else if (Input::Get().InputKey(INPUT_RIGHT))
	{
		reverse = false;
		SetState("walk");
	}

	if (Input::Get().InputKey(INPUT_UP) | Input::Get().InputKey(INPUT_DOWN))
	{
		SetState("walk");
	}
	else
	{
	}
}

// 歩行時の処理
void Player::Walk(void)
{
	if (state != "walk")
	{
		return;
	}

	// 
	bool walking = false;


	if (Input::Get().InputKey(INPUT_X))
	{
		SetState("sliding");
		walking = false;
		return;
	}

	if (Input::Get().InputKey(INPUT_RIGHT))
	{
		reverse = false;
		pos.x += speed;
		walking = true;
	}
	 if (Input::Get().InputKey(INPUT_LEFT))
	{
		reverse = true;
		pos.x -= speed;
		walking = true;
	}
	 if (Input::Get().InputKey(INPUT_UP))
	{
		pos.y -= speed;
		walking = true;
	}
	 if (Input::Get().InputKey(INPUT_DOWN))
	{
		pos.y += speed;
		walking = true;
	}
	if(!walking)
	{
		SetState("wait");
	}
}

// 回避時の処理
void Player::Avoid(void)
{
	if (!reverse)
	{
		pos.x += speed * 3.4;
	}
	else
	{
		pos.x -= speed * 3.4;
	}
	if (state != "avoid")
	{
		return;
	}	
	if (CheckAnimEnd())
	{
		SetState("wait");
	}


}

// 攻撃2時の処理
void Player::Attack1(void)
{
	// 追撃を行うか
	static bool atk_flg = false;
	if (state != "attack1")
	{
		return;
	}
	if (Input::Get().Triger(INPUT_Z))
	{
		atk_flg = true;
	}
	if (CheckAnimEnd())
	{
		SetState((atk_flg == true ? "attack2" : "wait"));
		atk_flg = false;
	}
}

// 攻撃2時の処理
void Player::Attack2(void)
{
	if (state != "attack2")
	{
		return;
	}
	if (CheckAnimEnd())
	{
		SetState("wait");
	}

}

// スライディング時の処理
void Player::Sliding(void)
{
	if (state != "sliding")
	{
		return;
	}
	

	if (!reverse)
	{
		pos.x += speed * 3.4;
	}
	else
	{
		pos.x -= speed * 3.4;
	}

	if (Input::Get().InputKey(INPUT_Z))
	{
		SetState("avoid");
		return;
	}

	if (CheckAnimEnd())
	{
		SetState("wait");
	}

}
// ダメージ時の処理
void Player::Damage(void)
{
	if (state != "damage")
	{
		return;
	}	
	if (CheckAnimEnd())
	{
		SetState("wait");
	}
}

// 描画
void Player::Draw(void)
{
	DrawImg("pl");

#if _DEBUG
	DrawRect();
#endif
}

// 処理
void Player::UpData(void)
{
	UpDataLocal();
	Animator();
	func[state](this);
}
