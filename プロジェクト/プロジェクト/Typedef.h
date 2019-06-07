#pragma once
#include <MyLib.h>
#include <vector>
#include <string>

//矩形
template <typename T>
struct Rect
{
	T pos;
	T size;
};

// 矩形種類
enum class HitType
{
	Damage,
	Attack
};

// 衝突矩形
template <typename T>
struct HitRect
{
	HitType type;
	Rect<T> rect;
};

// アニメーション切り取り矩形、衝突矩形
struct AnimHitRect
{
	Rect<Vec2f> anim;
	std::vector<HitRect<Vec2f>> hit;
};

// 情報
struct Data
{
	std::string state;
	float animTime;
	std::vector<AnimHitRect> rect;
};

// ステージ関係
