#pragma once
#include <MyLib.h>
#include <memory>
#include <vector>

class Camera;

class BackGround
{
public:
	BackGround(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~BackGround();

	void Update();
	void Draw();
private:
	std::vector<Texture> tex;
	std::weak_ptr<MyLib> lib;
	std::weak_ptr<Camera> cam;
	bool turnFlag;
};