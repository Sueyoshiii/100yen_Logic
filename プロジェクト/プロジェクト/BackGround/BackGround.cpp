#include "BackGround.h"
#include "../Camera/Camera.h"

BackGround::BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	lib(lib), cam(cam), turnFlag(false)
{
	tex.resize(4);
	std::fill(tex.begin(), tex.end(), Texture("img/birdes.jpg"));
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
}

void BackGround::Draw()
{
	int scroll = int(cam.lock()->GetViewPort().Left());
	scroll %= int(tex.begin()->size.x);
	for (int i = 0; i < tex.size(); ++i)
	{
		tex[i].pos.x = (tex[i].size.x * i) - scroll;
		lib.lock()->Draw(tex[i], 1.0f, turnFlag);
		turnFlag = !turnFlag;
	}
}
