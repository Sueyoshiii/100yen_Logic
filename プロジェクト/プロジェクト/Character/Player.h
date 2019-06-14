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

	// ワールド座標取得
	Vec2f GetWorldPos()const;
private:
	// 状態と関数をバインド
	void InitFunc();
	// 待機
	void NeutralUpdate();
	// 歩行
	void WalkUpdate();
	void CheckWalk();
	// ジャンプ
	void JumpUpdate();
	void CheckJump();
	// 落下
	void FallUpdate();
	void CheckFall();
	// ダッシュ
	void DashUpdate();
	void CheckDash();
	// 最初の攻撃
	void FirstAttackUpdate();
	void CheckFirstAttack();
	// 2段目の攻撃
	void SecondAttackUpdate();
	// 3段目の攻撃
	void ThirdAttackUpdate();
	// 次の攻撃へ移る
	void CheckNextAttack(const unsigned int attackInterval);
	// ダメージ
	void DamageUpdate();
	// 死亡
	void DeathUpdate();

	// ジャンプフラグ
	bool jumpFlag;

	// ダッシュフラグ
	bool dashFlag;

	// 攻撃フラグ
	bool attackFlag;

	// 攻撃カウント
	unsigned int attackCnt;
};