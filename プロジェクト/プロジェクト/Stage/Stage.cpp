#include "Stage.h"

const float Stage::Const::GR = 1.9f;
const float Stage::Const::GROUND = 1000.0f;

// �R���X�g���N�^
Stage::Stage()
{
	range = Stage::Rect(Vec2f(0, 0), 640 * 10, 640);
}

// �f�X�g���N�^
Stage::~Stage()
{
}

// �C���X�^���X
Stage& Stage::Get()
{
	static Stage instance;
	return instance;
}

// �X�e�[�W�f�[�^�ǂݍ���
int Stage::Load(const std::string& filePath)
{
	return 0;
}

// �͈͎擾
StageRange Stage::GetRange() const
{
	StageRange stRange = StageRange(range.Top(), range.Bottom(), range.Left() / 10, range.Right());
	return stRange;
}

// �n�ʎ擾
float Stage::GetGround() const
{
	return Const::GROUND;
}

// �d�͎擾
float Stage::GetGravity() const
{
	return Const::GR;
}
