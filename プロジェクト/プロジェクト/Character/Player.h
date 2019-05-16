#pragma once
#include "Character.h"
#include <string>
#include <memory>
#include <MyLib.h>

class Camera;

// 定数
typedef
struct ConstParam
{
	static const float SPEED;		// 速度
	static const float JUMP_POW;	// ジャンプ力
	static const float GR;			// 重力
	static const float GROUND;		// 地面
}Const;

class Player : 
	public Character
{
public:
	Player(std::weak_ptr<MyLib> lib, std::weak_ptr<Camera> cam);
	~Player();

	// 更新
	void Update();

	// 描画
	void Draw();

	Vec2f GetLocalPos()const;
private:
	// 状態
	void(Player::* update)();
	// 待機
	void NeutralUpdate();
	// 歩行
	void WalkUpdate();
	// ジャンプ
	void JumpUpdate();
	void Jump();

	// ジャンプフラグ
	bool jumpFlag;

	// 座標
	Vec2f pos;

	std::weak_ptr<Camera> cam;
};