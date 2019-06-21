#pragma once
#include "CharaEffect.h"

class Flower :
	public CharaEffect
{
public:
	~Flower();

	static Flower& Get();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 生成
	void Create(const Vec2f& inPos);
private:
	Flower();
	Flower(const Flower&) = delete;
	void operator=(const Flower&) = delete;

	std::vector<Texture> texs;
};
