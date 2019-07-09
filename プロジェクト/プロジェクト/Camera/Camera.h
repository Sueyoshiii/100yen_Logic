#pragma once
#include <MyLib.h>
#include <memory>

class Player;

struct ViewRange
{
	float left;
	float right;

	ViewRange() : left(0.0f), right(0.0f) {}
	ViewRange(const Vec2f& pos, const Vec2f& size)
	{
		left  = pos.x - size.x;
		right = pos.x + size.x;
	}
	
	float GetLeft()const { return left; }
	float GetRight()const { return right; }
};

class Camera
{
public:
	Camera(std::weak_ptr<MyLib> lib);
	~Camera();

	// 更新
	void Update();

	// 
	Vec2f Correction(const Vec2f& pos);

	// 座標取得
	Vec2f GetPos()const;

	ViewRange GetRange()const;

	// 対象を設定
	void SetFocus(std::weak_ptr<Player> pl);

	// 振動フラグを設定
	void SetVibrationFlag(const bool flag);
private:
	// 座標
	Vec2f pos;

	// サイズ
	Vec2f size;

	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// プレイヤー
	std::weak_ptr<Player> pl;

	// 振動フラグ
	bool vibrationFlag;

	Vec2f fulcrum;

	float speed;

	unsigned int vibrationCnt;
};