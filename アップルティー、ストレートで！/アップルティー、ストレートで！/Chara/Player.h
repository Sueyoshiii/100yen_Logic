#pragma once
#include "Chara.h"

class Player :
	public Chara
{
public:
	// コンストラクタ
	Player(std::weak_ptr<Application> app, std::weak_ptr<Camera>cam, const Vec2f & pos, const Vec2f & size);
	// デストラクタ
	~Player();

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

	// 回避時の処理
	void Avoid(void);

	// 攻撃1時の処理
	void Attack1(void);

	// 攻撃2時の処理
	void Attack2(void);

	// スライディング時の処理
	void Sliding(void);

	// ダメージ時の処理
	void Damage(void);


	// 関数ポインタ
	std::unordered_map<std::string, std::function<void(Player*)>>func;
};
