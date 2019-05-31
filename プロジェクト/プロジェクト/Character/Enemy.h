#pragma once
#include "Character.h"
#include "Player.h"

// “G‚³‚ñ’B‚Ì‚¨–¼‘O
typedef
enum class Enemies
{
	Wolf
}EM;

// ü•ª\‘¢‘Ì
struct Segment
{
	// n“_
	Vec2f begin;
	// I“_
	Vec2f end;
	// •ûŒüƒxƒNƒgƒ‹
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

	// Õ“Ë”»’è
	void CheckHit();

	// ‹ŠE”»’è
	void CheckView();

	// ü•ª“¯m‚ÌÕ“Ë”»’è
	bool CheckColSegments(const Segment& seg1, const Segment& seg2);

	// ‹ŠE•`‰æ
	void DrawViewRange();

	// ó‘Ô‰Šú‰»
	virtual void InitFunc() = 0;

	// ƒvƒŒƒCƒ„[
	std::weak_ptr<Player> pl;

	// ‹ŠE
	Vec2f viewRange;

	// ‹ŠE”ÍˆÍ
	Primitive viewBox;

	// ‹ŠE
	Primitive viewLine;
	Primitive viewLine2;

	// ‹ŠEü•ª
	Segment seg;
};