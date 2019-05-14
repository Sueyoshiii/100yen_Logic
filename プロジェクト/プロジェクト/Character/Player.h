#pragma once
#include "Character.h"
#include <string>
#include <memory>
#include <MyLib.h>

class Player : 
	public Character
{
public:
	Player(std::weak_ptr<MyLib> lib);
	~Player();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 座標取得
	Vec2f GetPos()const;

	// サイズ取得
	Vec2f GetSize()const;
private:
	// 待機
	void NeutralUpdate();
	// 歩行
	void WalkUpdate();
	// 状態
	void(Player::* update)();

	// プレイヤー
	Texture pl;

	// 反転フラグ
	bool turnFlag;
};