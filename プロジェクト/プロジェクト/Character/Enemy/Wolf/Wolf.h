#pragma once
#include "../Enemy.h"

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
	// 歩き
	void WalkUpdate();
	void CheckWalk();
	// 咆哮
	void HowlingUpdate();
	void CheckHowling();
	// 威嚇
	void ThreatUpdate();
	void CheckTreat();
	// 追いかける
	void RunUpdate();
	void CheckRun();
	// 溜め
	void SaveUpdate();
	void CheckSave();
	// 攻撃
	void AttackUpdate();
	void CheckAttack();
	// 被ダメージ
	void DamageUpdate();
	// 死亡
	void DeathUpdate();

	// 支点
	Vec2f fulcrum;

	// クールタイム
	unsigned int coolTime;

	// 発見フラグ
	bool discovery;

	unsigned int cnt;

	bool coolFlag;
};