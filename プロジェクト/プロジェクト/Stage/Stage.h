#pragma once
#include <MyLib.h>

class Camera;


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

	// 定数
	typedef
		struct ConstParam
	{
		// 重力
		static const float GR;
		// 地面
		static const float GROUND;
	}Const;
public:
	~Stage();
	static Stage& Get();

	// ステージデータ読み込み
	int Load(const std::string& filePath);

	// ステージ範囲取得
	StageRange GetRange()const;

	// 地面取得
	float GetGround()const;

	// 重力取得
	float GetGravity()const;
private:
	Stage();
	Stage(const Stage&) = delete;
	void operator=(const Stage&) = delete;

	Rect range;
};