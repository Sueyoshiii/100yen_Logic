#pragma once
#include "../etc/Vector2.h"
#include <vector>
#include <memory>

class Application;
class Camera;

class BackGround
{
public:
	// コンストラクタ
	BackGround(std::weak_ptr<Application>app, std::weak_ptr<Camera>cam);
	// デストラクタ
	~BackGround();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 初期化
	void Init(void);


	// アプリケーション
	std::weak_ptr<Application>app;
	
	// カメラ
	std::weak_ptr<Camera>cam;

	// 画像ID
	std::vector<int>image;

	// 座標
	std::vector<Vec2f>pos;

	// サイズ
	Vec2f size;
};
