#include "Flower.h"
#include <random>

// �R���X�g���N�^
Flower::Flower(const Vec2f& pos)
{
	texs.resize(3);
	texs[0].Load("img/Item/itemB.png");
	texs[1].Load("img/Item/itemR.png");
	texs[2].Load("img/Item/itemY.png");

	for (auto& t : texs)
	{
		t.pos  = pos;
		t.size = Vec2f(64.0f, 64.0f);
	}
}

// �f�X�g���N�^
Flower::~Flower()
{
	texs.clear();
}

// �X�V
void Flower::Update()
{
	if (texs.size() == 0)
	{
		return;
	}

	texs[0].pos.x -= 50.0f;
	texs[1].pos.x += 50.0f;
	texs[2].pos.y -= 50.0f;
}

// �`��
void Flower::Draw()
{
}
void Flower::Draw(std::weak_ptr<MyLib> lib)
{
	for (auto& t : texs)
	{
		lib.lock()->Draw(t);
	}
}

// �폜
void Flower::Delete()
{
}
