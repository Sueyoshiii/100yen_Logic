#include "Camera.h"
#include "../Character/Player.h"

Camera::Camera()
{
	this->range = Rect(Vec2f(), 640 * 2, 640);
}
Camera::Camera(const Rect& range) :
	range(range)
{
	this->range = Rect(Vec2f(), 640 * 2, 640);
}

Camera::~Camera()
{
}

// 更新
void Camera::Update()
{
	Vec2f size = Vec2f(640, 640);
	pos = pl.lock()->GetPos();

	//カメラ座標の補正
	if (pos.x - size.x / 2 < range.Left()) {
		pos.x = range.Left() + size.x / 2;
	}
	else if (pos.x + size.x / 2 > range.Right()) {
		pos.x = range.Right() - size.x / 2;
	}

	if (pos.x < 0.0f)
	{
		pos.x = 0.0f;
	}
}

Vec2f Camera::Correction(const Vec2f& pos)
{
	Vec2f tmp = pos;
	tmp.x -= this->pos.x - 640;

	return tmp;
}

// 座標取得
Vec2f Camera::GetPos()const
{
	return pos;
}

Rect Camera::GetViewPort() const
{
	return range;
}

void Camera::SetFocus(std::weak_ptr<Player> pl)
{
	this->pl = pl;
}
