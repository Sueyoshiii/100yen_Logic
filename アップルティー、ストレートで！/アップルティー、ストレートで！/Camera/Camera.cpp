#include "Camera.h"
#include "../Chara/Chara.h"
#include "../Application/Application.h"

// コンストラクタ
Camera::Camera(std::weak_ptr<Application> app) : app(app),
	pos(0.0f)
{
}

// デストラクタ
Camera::~Camera()
{
}

// 処理
void Camera::UpData(void)
{
	static float moveCnt = 0.0f;
	float left  = app.lock()->GetWinSize().x / 8.0f;
	float right = app.lock()->GetWinSize().x - app.lock()->GetWinSize().x / 4.0f;

	if (target.lock()->GetPos().x + moveCnt < left)
	{
		moveCnt -= (target.lock()->GetPos().x + moveCnt - left) / 16.0f;
	}
	else if(target.lock()->GetPos().x + moveCnt > right)
	{
		moveCnt -= (target.lock()->GetPos().x + moveCnt - right) / 16.0f;
	}
	else{}

	pos.x = moveCnt;

}