#include "Camera.h"
#include "../Stage/StageManager.h"
#include "../Character/Player/Player.h"

namespace {
	const unsigned int VIBRATION_CNT_MAX = 4;
	const float FULCRUM_RANGE = 5.0f;
}

// コンストラクタ
Camera::Camera(std::weak_ptr<MyLib> lib) :
	lib(lib), size(Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y))),
	vibrationFlag(false), speed(5.0f), vibrationCnt(0)
{
}

// デストラクタ
Camera::~Camera()
{
}

// 更新
void Camera::Update()
{
	if (vibrationFlag)
	{
		if (vibrationCnt > VIBRATION_CNT_MAX)
		{
			vibrationCnt = 0;
			vibrationFlag = false;
		}
		else
		{
			pos.x += speed;
			if (fulcrum.x + FULCRUM_RANGE <= pos.x ||
				fulcrum.x - FULCRUM_RANGE >= pos.x)
			{
				speed = -speed;
				++vibrationCnt;
			}
		}
	}
	else
	{
		SetPos(pl.lock()->GetWorldPos());
	}
}

// 受け取った座標を補正
Vec2f Camera::Correction(const Vec2f& pos)
{
	return Vec2f(pos.x - (this->pos.x - size.x / 2), pos.y);
}

// 座標取得
Vec2f Camera::GetPos()const
{
	return pos;
}

ViewRange Camera::GetRange() const
{
	return ViewRange(pos, size);
}

// 対象を設定
void Camera::SetFocus(std::weak_ptr<Player> pl)
{
	this->pl = pl;
}

// 振動フラグを設定
void Camera::SetVibrationFlag(const bool flag)
{
	vibrationFlag = flag;
	fulcrum = pos;
}

// 座標の設定
void Camera::SetPos(const Vec2f& pos)
{
	if (this->pos.x < pos.x)
	{
		this->pos.x += 4.0f;
	}
	else if (this->pos.x > pos.x)
	{
		this->pos.x -= 4.0f;
	}
	else
	{
		this->pos = pos;
	}

	float left   = StageManager::Get().GetRange().Left();
	float right  = StageManager::Get().GetRange().Right();
	float top    = StageManager::Get().GetRange().Top();
	float bottom = StageManager::Get().GetRange().Bottom();

	if (this->pos.x - size.x / 2 < left) {
		this->pos.x = left + size.x / 2;
	}
	else if (this->pos.x + size.x / 2 > right) {
		this->pos.x = right - size.x / 2;
	}

	if (this->pos.x < 0.0f)
	{
		this->pos.x = 0.0f;
	}

	if (this->pos.y - size.y / 2 < top) {
		this->pos.y = top + size.y / 2;
	}
	else if (this->pos.y + size.y / 2 > bottom) {
		this->pos.y = bottom - size.y / 2;
	}

	if (this->pos.y < 0.0f)
	{
		this->pos.y = 0.0f;
	}
}
