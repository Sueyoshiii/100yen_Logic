#pragma once
#include <MyLib.h>
#include <memory>

class Player;

class Camera
{
public:
	Camera();
	~Camera();

	// 更新
	void Update();

	// 
	Vec2f Correction(const Vec2f& pos);

	// 座標取得
	Vec2f GetPos()const;

	// 対象を設定
	void SetFocus(std::weak_ptr<Player> pl);
private:
	Vec2f pos;
	std::weak_ptr<Player> pl;
};