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
	// 状態初期化
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

	// ジャンプフラグ
	bool jumpFlag;

	// ダッシュフラグ
	bool dushFlag;

	// 座標
	Vec2f pos;

	std::weak_ptr<Camera> cam;
};