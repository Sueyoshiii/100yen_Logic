#pragma once
#include "../Character.h"

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

	// ヒットフラグ取得
	bool GetHitFlag();

	// 死亡フラグ取得
	bool GetDeadEndFlag()const;

	// 初期座標取得
	Vec2f GetFirstPos()const;

	// ヒットフラグを設定
	void SetHitFlag(const bool flag);

	// 座標を設定
	void SetPos(const Vec2f& pos);
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
	// 4段目の攻撃
	void FourthAttackUpdate();
	// 次の攻撃へ移る
	void CheckNextAttack(const unsigned int attackInterval);
	// ジャンプ攻撃
	void JumpAttackUpdate();
	void CheckJumpAttack();
	// ダメージ
	void DamageUpdate();
	// 死亡
	void DeathUpdate();
	// 変身
	void TransformUpdate();
	void CheckTransform();
	void Transform(const CharacterType& next, const CharacterType& now, const CharacterParameter& param);

	// ヒットストップ処理
	bool CheckHitStop();

	// ダメージ時の描画
	void DamageDraw();

	// ジャンプフラグ
	bool jumpFlag;

	// ダッシュフラグ
	bool dashFlag;

	// 攻撃フラグ
	bool attackFlag;

	// 攻撃カウント
	unsigned int attackCnt;

	// ヒットフラグ
	bool hitFlag;

	// ヒットストップカウント
	unsigned int hitStopCnt;

	// 消滅カウント
	unsigned int disappearCnt;

	// クリティカルダメージ
	Texture critical;

	// クリティカルダメージカウント
	unsigned int criticalCnt;

	// ダメージ演出のアルファ値
	float criticalAlpha;

	// 死亡フラグ
	bool deadEndFlag;

	// hp画像
	std::vector<Texture> hpImg;
};