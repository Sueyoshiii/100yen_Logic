#pragma once
#include "Enemy.h"

class Enemy2 :
	public Enemy 
{
public:
	// コンストラクタ
	Enemy2(std::weak_ptr<Application>app, std::weak_ptr<Camera> cam, std::weak_ptr<Player>pl, const Vec2f& pos = 0.0f, const Vec2f& size = 100.0f);
	// デストラクタ
	~Enemy2();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// ステータス毎の処理のセット
	void SetFunc(void);

	// 待機時の処理
	void Wait(void);

	// 歩行時の処理
	void Walk(void);

	// 攻撃時の処理
	void Attack(void);

	// ダメージ時の処理
	void Damage(void);

	// 死亡時の処理
	void Dead(void);


	// 関数ポインタ
	std::unordered_map<std::string, std::function<void(Enemy2*)>>func;
};