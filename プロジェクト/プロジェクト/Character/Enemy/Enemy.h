#pragma once
#include "../Character.h"
#include "../Player/Player.h"
#include "../CharaEffect/Slash/Slash.h"
#include <array>

// 敵さん達のお名前
enum class Enemies
{
	Wolf,
	BOSS_WOLF
};

// 矩形構造体
struct Box
{
	// 左上座標
	Vec2f pos;
	// サイズ
	Vec2f size;
	// 半分のサイズ
	Vec2f half;
	// 中心座標
	Vec2f centor;

	Box() : pos(Vec2f()), size(Vec2f()), centor(Vec2f()) {}
	Box(const Vec2f& pos, const Vec2f& size) 
	{
		this->pos = pos;
		this->size = size;
		half = size / 2.0f;
		centor = pos + half;
	}
};

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
	virtual ~Enemy();

	// 削除フラグ取得
	bool GetDeleteFlag()const;
protected:
	// 衝突判定
	void CheckHit();

	void CheckHitEffect();

	// 視界判定
	bool CheckView();

	// 矩形同士の衝突判定
	bool CheckColBox(const Box& box1, const Box& box2);
	bool CheckColBox(const Vec2f& pos1, const Vec2f& size1, const Vec2f& pos2, const Vec2f& size2);

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

	// 削除フラグ
	bool deleteFlag;

	// 気絶フラグ
	bool stunFlag;

	// ヒット数
	unsigned int hitCnt;
private:
};