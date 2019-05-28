#pragma once
#include "Scene.h"
#include <memory>
#include <MyLib.h>

class Camera;
class BackGround;
class Player;
class Wolf;

class GameMain :
	public Scene
{
public:
	// コンストラクタ
	GameMain(std::weak_ptr<MyLib>lib);
	// デストラクタ
	~GameMain();

	// 描画
	void Draw(void);
	
	// 処理
	void UpData(void);

private:
	// カメラ
	std::shared_ptr<Camera> cam;

	// 背景
	std::shared_ptr<BackGround> bg;

	// プレイヤー
	std::shared_ptr<Player> pl;

	// 狼
	// 最終的には敵は一括管理
	std::shared_ptr<Wolf> wolf;
};
