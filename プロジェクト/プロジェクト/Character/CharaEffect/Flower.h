#pragma once
#include "CharaEffect.h"

class Flower :
	public CharaEffect
{
public:
	Flower(const Vec2f& pos);
	~Flower();

	// XV
	void Update();

	// •`‰æ
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	// íœ
	void Delete();
private:
	std::vector<Texture> texs;
};
