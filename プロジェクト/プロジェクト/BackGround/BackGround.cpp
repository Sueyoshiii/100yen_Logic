#include "BackGround.h"
#include "../Camera/Camera.h"
#include <algorithm>

BackGround::BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam) :
	lib(lib), cam(cam), turnFlag(false)
{
	tex.resize(2);
	for (auto& i : tex)
	{
		i.Load("img/brides.jpg");
	}
	//(*tex.begin()) = Texture("img/brides.jpg");
	
	//std::fill(&tex[1], &tex[3], tex[0]);
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
}

void BackGround::Draw()
{
	for (size_t i = 0; i < tex.size(); ++i)
	{
		tex[i].pos.x = (tex[i].size.x * i) - int(cam.lock()->GetPos().x) % int(tex[i].size.x);
		lib.lock()->Draw(tex[i], 1.0f, turnFlag);
		//turnFlag = !turnFlag;
	}

	/*int scroll = int(cam.lock()->GetViewPort().Left());
	scroll %= int(tex.begin()->size.x);
	for (int i = 0; i < tex.size(); ++i)
	{
		tex[i].pos.x = (tex[i].size.x * i) - scroll;
		lib.lock()->Draw(tex[i], 1.0f, turnFlag);
		turnFlag = !turnFlag;
	}*/
}
