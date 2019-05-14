#pragma once
#include <memory>

class MyLib;

#define In Input::Get()

class Character
{
public:
	Character();
	~Character();

	virtual void Update() = 0;
	virtual void Draw() = 0;
protected:
	std::weak_ptr<MyLib> lib;
};