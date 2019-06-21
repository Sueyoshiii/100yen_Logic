#include "Flower.h"
#include <random>

// �R���X�g���N�^
Flower::Flower()
{
	texs.push_back(Texture("itemB.png"));
	texs.push_back(Texture("itemR.png"));
	texs.push_back(Texture("itemY.png"));


}

// �f�X�g���N�^
Flower::~Flower()
{
}

// �C���X�^���X
Flower& Flower::Get()
{
	static Flower instance;
	return instance;
}

// �X�V
void Flower::Update()
{
}

// �`��
void Flower::Draw()
{
}

// ����
void Flower::Create(const Vec2f& inPos)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	auto index = mt() % unsigned int(texs.size());

	texs[index].pos = inPos;
}
