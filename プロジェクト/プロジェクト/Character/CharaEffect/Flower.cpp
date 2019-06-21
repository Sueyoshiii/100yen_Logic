#include "Flower.h"
#include <random>

// コンストラクタ
Flower::Flower()
{
	texs.push_back(Texture("itemB.png"));
	texs.push_back(Texture("itemR.png"));
	texs.push_back(Texture("itemY.png"));


}

// デストラクタ
Flower::~Flower()
{
}

// インスタンス
Flower& Flower::Get()
{
	static Flower instance;
	return instance;
}

// 更新
void Flower::Update()
{
}

// 描画
void Flower::Draw()
{
}

// 生成
void Flower::Create(const Vec2f& inPos)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	auto index = mt() % unsigned int(texs.size());

	texs[index].pos = inPos;
}
