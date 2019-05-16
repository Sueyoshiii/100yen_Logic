#pragma once
#include <MyLib.h>

class Camera;

struct Rect
{
	Vec2f pos;
	float w, h;
	Rect() : pos(Vec2f()), w(0.0f), h(0.0f) {};
	Rect(const Vec2f& pos, const float w, const float h) :
		pos(pos), w(w), h(h) {};
	float Top()const { return pos.y - h / 2.0f; };
	float Bottom()const { return pos.y + h / 2.0f; };
	float Left()const { return pos.x - w / 2.0f; };
	float Right()const { return pos.x + w / 2.0f; };
};

struct StageRange
{
private:
	float top, bottom, left, right;
public:
	StageRange() : top(0.0f), bottom(0.0f), left(0.0f), right(0.0f) {};
	StageRange(const float top, const float bottom, const float left, const float right) :
		top(top), bottom(bottom), left(left), right(right) {};
	float Top()const { return top; };
	float Bottom()const { return bottom; };
	float Left()const { return left; };
	float Right()const { return right; };
};

class Stage
{
public:
	~Stage();
	static Stage& Get();

	StageRange GetRange()const;
private:
	Stage();
	Stage(const Stage&) = delete;
	void operator=(const Stage&) = delete;

	Rect range;
};