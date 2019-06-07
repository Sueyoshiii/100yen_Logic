#pragma once
#include <MyLib.h>
#include <vector>
#include <string>

//��`
template <typename T>
struct Rect
{
	T pos;
	T size;
};

// ��`���
enum class HitType
{
	Damage,
	Attack
};

// �Փˋ�`
template <typename T>
struct HitRect
{
	HitType type;
	Rect<T> rect;
};

// �A�j���[�V�����؂����`�A�Փˋ�`
struct AnimHitRect
{
	Rect<Vec2f> anim;
	std::vector<HitRect<Vec2f>> hit;
};

// ���
struct Data
{
	std::string state;
	float animTime;
	std::vector<AnimHitRect> rect;
};

// �X�e�[�W�֌W
