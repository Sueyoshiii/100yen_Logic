#pragma once
#include "Scene.h"
#include <memory>
#include <MyLib.h>

class Camera;
class BackGround;
class Player;

class GameMain :
	public Scene
{
public:
	// コンストラクタ
	GameMain(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~GameMain();

	// 描画
	void Draw();
	
	// 処理
	void UpData();

private:
	// カメラ
	std::shared_ptr<Camera> cam;

	// 背景
	std::shared_ptr<BackGround> bg;

	// プレイヤー
	std::shared_ptr<Player> pl;
};
