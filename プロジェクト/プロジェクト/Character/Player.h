#pragma once
#include "Character.h"

class Camera;


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
	// 状態と関数をバインド
	void InitFunc();
	// 待機
	void NeutralUpdate();
	// 歩行
	void WalkUpdate();
	void Walk();
	// ジャンプ
	void JumpUpdate();
	void Jump();
	// ダッシュ
	void DushUpdate();
	void Dush();
	// 攻撃1
	void Attack1Update();
	// 攻撃2
	void Attack2Update();
	// 攻撃3
	void Attack3Update();
	// ダメージ
	void DamageUpdate();
	// 死亡
	void DeathUpdate();

	// ジャンプフラグ
	bool jumpFlag;

	// ダッシュフラグ
	bool dushFlag;

	std::weak_ptr<Camera> cam;
};