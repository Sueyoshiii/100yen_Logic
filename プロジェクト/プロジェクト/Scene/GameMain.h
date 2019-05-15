#pragma once
#include "Scene.h"
#include <memory>
#include <MyLib.h>

class Camera;
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
	void Draw(void);
	
	// 処理
	void UpData(void);

private:
	// カメラ
	std::shared_ptr<Camera> cam;

	// プレイヤー
	std::shared_ptr<Player> pl;

	// 仮背景
	Texture t;
};
