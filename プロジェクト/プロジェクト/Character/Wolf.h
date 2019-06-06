#pragma once
#include "Enemy.h"

class Wolf : 
	public Enemy
{
public:
	Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos);
	~Wolf();
	
	// 更新
	void Update();

	// 描画
	void Draw();
private:
	// 状態初期化
	void InitFunc();
	// 待機
	void NeutralUpdate();
	// 徘徊
	void Loitering();
	// 発見
	void Alert();
	// 歩き
	void WalkUpdate();
	void Walk();
	// 攻撃
	void AttackUpdate();
	void Attack();
	// 被ダメージ
	void DamageUpdate();
	// 死亡
	void DeathUpdate();

	// 支点
	Vec2f fulcrum;

	// クールタイム
	unsigned int coolTime;
};