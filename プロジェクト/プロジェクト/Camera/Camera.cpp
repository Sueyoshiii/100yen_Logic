#include "Camera.h"
#include "../Stage/Stage.h"
#include "../Character/Player/Player.h"

namespace {
	const unsigned int VIBRATION_CNT_MAX = 3;
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
		pos = pl.lock()->GetWorldPos();

		//カメラ座標の補正
		float left = Stage::Get().GetRange().Left();
		float right = Stage::Get().GetRange().Right();
		if (pos.x - size.x / 2 < left) {
			pos.x = left + size.x / 2;
		}
		else if (pos.x + size.x / 2 > right) {
			pos.x = right - size.x / 2;
		}

		if (pos.x < 0.0f)
		{
			pos.x = 0.0f;
		}
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
