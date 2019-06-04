#pragma once
#include "Character.h"
#include "Player.h"
#include <array>

// G³ρBΜ¨ΌO
typedef
enum class Enemies
{
	Wolf
}EM;

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
	~Enemy();
protected:
	virtual void Walk() = 0;

	// ΥΛ»θ
	void CheckHit();

	// E»θ
	bool CheckView();

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
};