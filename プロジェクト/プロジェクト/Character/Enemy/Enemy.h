#pragma once
#include "../Character.h"
#include "../Player/Player.h"
#include "../CharaEffect/Slash/Slash.h"
#include <array>

// G³ρBΜ¨ΌO
enum class Enemies
{
	Wolf,
	BOSS_WOLF
};

// ι`\’Μ
struct Box
{
	// ΆγΐW
	Vec2f pos;
	// TCY
	Vec2f size;
	// ΌͺΜTCY
	Vec2f half;
	// SΐW
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

// όͺ\’Μ
struct Segment
{
	// n_
	Vec2f begin;
	// I_
	Vec2f end;
	// ϋόxNg
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

	// νtOζΎ
	bool GetDeleteFlag()const;
protected:
	// ΥΛ»θ
	void CheckHit();

	void CheckHitEffect();

	// E»θ
	bool CheckView();

	// ι`―mΜΥΛ»θ
	bool CheckColBox(const Box& box1, const Box& box2);
	bool CheckColBox(const Vec2f& pos1, const Vec2f& size1, const Vec2f& pos2, const Vec2f& size2);

	// όͺ―mΜΥΛ»θ
	bool CheckColSegments(const Segment& seg1, const Segment& seg2);

	// E`ζ
	void DrawViewRange();

	// σΤϊ»
	virtual void InitFunc() = 0;

	// vC[
	std::weak_ptr<Player> pl;

	// E
	Vec2f viewRange;

	// EΝΝ
	Primitive viewBox;

	// E
	Primitive viewLine;
	Primitive viewLine2;

	// Eόͺ
	Segment seg;

	// PlayerΜι`όͺ
	std::array<Segment, 4> pSeg;

	// PlayerΜOΐW
	Vec2f oldPlPos;

	// νtO
	bool deleteFlag;

	// CβtO
	bool stunFlag;

	// qbg
	unsigned int hitCnt;
private:
};