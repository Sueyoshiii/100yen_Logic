#include "BackGround.h"
#include "../Camera/Camera.h"
#include <algorithm>

// コンストラクタ
BackGround::BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	lib(lib), cam(cam), turnFlag(false)
{
	tex.resize(2);
	for (auto& i : tex)
	{
		i.Load("img/haikei.png");
		i.size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 1.8f);
	}
}

// デストラクタ
BackGround::~BackGround()
{
}

// 更新
void BackGround::Update()
{
}

// 秒が
void BackGround::Draw()
{
	for (size_t i = 0; i < tex.size(); ++i)
	{
		tex[i].pos.x = (tex[i].size.x * i) - int(cam.lock()->GetPos().x) % int(tex[i].size.x);
		lib.lock()->Draw(tex[i], 1.0f, turnFlag);
	}
}
