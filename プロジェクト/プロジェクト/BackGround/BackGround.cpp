#include "BackGround.h"
#include "../Camera/Camera.h"

namespace {
	const unsigned int LAYER_MAX = 4;
	std::array<std::string, LAYER_MAX> path;
}

// コンストラクタ
BackGround::BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	lib(lib), cam(cam), turnFlag(false)
{
	std::string name = "img/Stage/back_";
	unsigned int num = 1;
	std::string type = ".png";

	layer.resize(LAYER_MAX);
	for (auto& i : layer)
	{
		for (unsigned int j = 0; j < i.tex.size(); ++j)
		{
			i.tex[j].Load(name + std::to_string(num) + type);
			i.tex[j].size = Vec2f(float(lib.lock()->GetWinSize().x), float(lib.lock()->GetWinSize().y) * 2.0f);
			i.oldPos[j]   = i.tex[j].pos;
		}
		++num;
	}

	oldCamPos = cam.lock()->GetPos();
	offset    = 5.0f;
}

// デストラクタ
BackGround::~BackGround()
{
}

// 更新
void BackGround::Update()
{
	for (unsigned int i = 0; i < layer.size() - 1; ++i)
	{
		for (unsigned int j = 0; j < layer[i].tex.size(); ++j)
		{
			float v = i < layer.size() - 2 ? -cam.lock()->GetPos().x : (oldCamPos.x - cam.lock()->GetPos().x) / offset;
			layer[i].tex[j].pos.x = (layer[i].tex[j].size.x * j) + int(v) % int(layer[i].tex[j].size.x);
		}
	}
}

// 描画
void BackGround::Draw()
{
	for (int i = int(layer.size() - 1); i >= 0; --i)
	{
		for (auto& t : layer[i].tex)
		{
			lib.lock()->Draw(t, 1.0f, turnFlag);
		}
	}
}
