#include "BackGround.h"
#include "../Camera/Camera.h"
#include <algorithm>

// コンストラクタ
BackGround::BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	lib(lib), cam(cam), turnFlag(false)
{
	//tex.resize(2);
	//for (auto& i : tex)
	//{
	//	i.Load("img/Stage/haikei.png");
	//	i.size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 2.0f);
	//}

	layer1.resize(2);
	layer2.resize(2);
	layer3.resize(2);

	for (unsigned int i = 0; i < layer1.size(); ++i)
	{
		layer1[i].Load("img/Stage/back1.png");
		layer2[i].Load("img/Stage/back2.png");
		layer3[i].Load("img/Stage/back3.png");

		layer1[i].size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 2.0f);
		layer2[i].size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 2.0f);
		layer3[i].size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 2.0f);
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

// 描画
void BackGround::Draw()
{
	//for (size_t i = 0; i < tex.size(); ++i)
	//{
	//	tex[i].pos.x = (tex[i].size.x * i) - int(cam.lock()->GetPos().x) % int(tex[i].size.x);
	//	lib.lock()->Draw(tex[i], 1.0f, turnFlag);
	//}
}
