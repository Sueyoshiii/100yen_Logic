#pragma once
#include <MyLib.h>
#include <memory>

class Player;

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

	// 対象を設定
	void SetFocus(std::weak_ptr<Player> pl);
private:
	// 座標
	Vec2f pos;

	// サイズ
	Vec2f size;

	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// プレイヤー
	std::weak_ptr<Player> pl;
};