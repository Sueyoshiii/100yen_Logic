#pragma once
#include "Character.h"
#include "Player.h"
#include <array>

// 敵さん達のお名前
typedef
enum class Enemies
{
	Wolf
}EM;

// 線分構造体
struct Segment
{
	// 始点
	Vec2f begin;
	// 終点
	Vec2f end;
	// 方向ベクトル
	Vec2f vec;

	Segment() : begin(Vec2f()), end(Vec2f()), vec(Vec2f()) {};
	Segment(const Vec2f& begin, const Vec2f& end, const Vec2f& vec) :
		begin(begin), end(end), vec(vec) {};
	Segment(const Vec2f& begin, const Vec2f& end) :
		begin(begin), end(end), vec(end - begin) {};
};

class Enemy : 
	public Character
{
public:
	Enemy();
	~Enemy();
protected:
	virtual void Walk() = 0;

	// 衝突判定
	void CheckHit();

	// 視界判定
	bool CheckView();

	// 線分同士の衝突判定
	bool CheckColSegments(const Segment& seg1, const Segment& seg2);

	// 視界描画
	void DrawViewRange();

	// 状態初期化
	virtual void InitFunc() = 0;

	// プレイヤー
	std::weak_ptr<Player> pl;

	// 視界
	Vec2f viewRange;

	// 視界範囲
	Primitive viewBox;

	// 視界
	Primitive viewLine;
	Primitive viewLine2;

	// 視界線分
	Segment seg;

	// Playerの矩形線分
	std::array<Segment, 4> pSeg;

	// Playerの前座標
	Vec2f oldPlPos;
};