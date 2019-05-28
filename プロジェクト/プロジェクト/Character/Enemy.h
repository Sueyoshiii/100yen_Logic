#pragma once
#include "Character.h"

class Player;

// 敵さん達
typedef
enum class Enemies
{
	Wolf
}EM;

class Enemy : 
	public Character
{
public:
	Enemy();
	~Enemy();
protected:
	// 衝突判定
	void CheckHit();

	// 状態初期化
	virtual void InitFunc() = 0;

	// プレイヤー
	std::weak_ptr<Player> pl;
};