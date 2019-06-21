#pragma once
#include "CharaEffect.h"

class Flower :
	public CharaEffect
{
public:
	~Flower();

	static Flower& Get();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// ����
	void Create(const Vec2f& inPos);
private:
	Flower();
	Flower(const Flower&) = delete;
	void operator=(const Flower&) = delete;

	std::vector<Texture> texs;
};
