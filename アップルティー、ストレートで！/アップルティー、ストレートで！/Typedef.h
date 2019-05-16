#pragma once
#include "etc/Vector2.h"
#include <string>
#include <vector>

// 矩形
template<typename T>
struct Rect {
	//座標
	T pos;
	//サイズ
	T size;
};

// あたり矩形の種類
enum class HitType {
	//ダメージ
	damage,
	//攻撃
	attack
};

// あたり矩形
template<typename T>
struct HitRect {
	//種類
	HitType type;
	//矩形
	Rect<T>rect;
};

// アニメーション切り取り矩形・あたり矩形
struct AnimHitRect {
	//アニメーション矩形
	Rect<Vec2f>anim;
	//あたり矩形
	std::vector<HitRect<Vec2f>>hit;
};

// アニメーション・矩形情報
struct Info {
	//ステータス
	std::string st;
	//アニメーション移行フレーム
	float animTime;
	//アニメーション切り取り矩形
	std::vector<AnimHitRect>rect;
};
