#pragma once
#include "../CharaEffect.h"

class Player;

class Slash :
	public CharaEffect
{
public:
	Slash(std::weak_ptr<Player> pl);
	~Slash();

	// XV
	void Update();

	// •`‰æ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);
private:
};