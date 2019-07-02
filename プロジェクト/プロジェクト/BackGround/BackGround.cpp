#include "BackGround.h"
#include "../Camera/Camera.h"

namespace {
	const unsigned int LAYER_MAX = 3;
}

// コンストラクタ
BackGround::BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	lib(lib), cam(cam), turnFlag(false)
{
	std::string str = "img/Stage/back";
	unsigned int num = 1;
	std::string type = ".png";
	layer.resize(LAYER_MAX);
	for (auto& i : layer)
	{
		for (unsigned int j = 0; j < i.tex.size(); ++j)
		{
			i.tex[j].Load(str + std::to_string(num) + type);
			i.tex[j].size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 2.0f);
			i.oldPos[j] = i.tex[j].pos;
		}
		++num;
	}

	oldCamPos = cam.lock()->GetPos();
	offset = 10.0f;
}

// デストラクタ
BackGround::~BackGround()
{
}

// 更新
void BackGround::Update()
{
	unsigned int index = 0;
	for (auto& i : layer)
	{
		for (unsigned int j = 0; j < i.tex.size(); ++j)
		{
			i.tex[j].pos.x = (i.tex[j].size.x * j) - int(cam.lock()->GetPos().x) % int(i.tex[j].size.x);
		}
		++index;
	}

	float v = (oldCamPos.x - cam.lock()->GetPos().x) / 10;
	layer[2].tex[0].pos.x = layer[2].oldPos[0].x + v;
	layer[2].tex[1].pos.x = layer[2].oldPos[1].x + v;
}

// 描画
void BackGround::Draw()
{
	for (int i = int(layer.size() - 1); i >= 0; --i)
	{
		lib.lock()->Draw(layer[i].tex[0], 1.0f, turnFlag);
		lib.lock()->Draw(layer[i].tex[1], 1.0f, turnFlag);
	}
}
