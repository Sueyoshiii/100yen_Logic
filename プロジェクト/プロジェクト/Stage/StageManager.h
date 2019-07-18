#pragma once
#include "Stage.h"

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

class StageManager
{
	// ステージ
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
public:
	~StageManager();
	static StageManager& Get();

	void Update();
	void Draw();
	void DrawBox();

	void SetRoom();
	void SetRoom(Stage* room);

	// ステージ範囲取得
	StageRange GetRange();

	bool GetNextRoomFlag()const;
	float GetBoxAlpha()const;

	void SetRange(const Vec2& size);
	void SetNextRoomFlag(const bool flag = true);

	float GetGround()const;
	float GetGravity()const;
private:
	StageManager();
	StageManager(const StageManager&) = delete;
	void operator=(const StageManager&) = delete;

	//範囲
	Rect range;

	std::shared_ptr<Stage> stage;
};