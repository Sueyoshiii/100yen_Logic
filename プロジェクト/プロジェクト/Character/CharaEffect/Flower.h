#pragma once
#include "CharaEffect.h"

class Flower :
	public CharaEffect
{
public:
	Flower(const Vec2f& pos);
	~Flower();

	// �X�V
	void Update();

	// �`��
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	// �폜
	void Delete();
private:
	std::vector<Texture> texs;
};
