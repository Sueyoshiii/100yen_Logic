#pragma once
#include "Scene.h"
#include <memory>

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
	// プレイヤー
	std::shared_ptr<Player> pl;
};
