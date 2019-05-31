#include "Camera.h"
#include "../Stage/Stage.h"
#include "../Character/Player.h"

Camera::Camera(std::weak_ptr<MyLib> lib) :
	lib(lib), size(Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y)))
{
}

Camera::~Camera()
{
}

// �X�V
void Camera::Update()
{
	pos = pl.lock()->GetWorldPos();

	//�J�������W�̕␳
	float left  = Stage::Get().GetRange().Left();
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

// �󂯎�������W���J�����͈͓̔��ɕ␳
Vec2f Camera::Correction(const Vec2f& pos)
{
	return Vec2f(pos.x - (this->pos.x - size.x / 2), pos.y);
}

// ���W�擾
Vec2f Camera::GetPos()const
{
	return pos;
}

void Camera::SetFocus(std::weak_ptr<Player> pl)
{
	this->pl = pl;
}
