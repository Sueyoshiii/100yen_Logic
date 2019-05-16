#pragma once
#include "../etc/Vector2.h"
#include <memory>

class Application;
class Chara;

class Camera
{
public:
	// コンストラクタ
	Camera(std::weak_ptr<Application> app);
	// デストラクタ
	~Camera();
	
	// 処理
	void UpData(void);

	// ターゲットのセット
	void SetTarget(std::weak_ptr<Chara>target) {
		this->target = target;
	}
	// 座標の取得
	Vec2f GetPos(void) const {
		return pos;
	}
	// 座標のセット
	void SetPos(const Vec2f& pos) {
		this->pos = pos;
	}

private:
	// アプリケーション
	std::weak_ptr<Application> app;

	// ターゲットキャラ
	std::weak_ptr<Chara>target;

	// 座標
	Vec2f pos;
};