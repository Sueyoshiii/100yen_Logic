#pragma once
#include "Scene.h"

class Camera;
class BackGround;
class Map;
class Player;
class EnemyMane;

class Play :
	public Scene
{
public:
	// コンストラクタ
	Play(std::weak_ptr<Application>app);
	// デストラクタ
	~Play();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 生成
	void Create(void);


	// カメラ
	std::shared_ptr<Camera>cam;

	// 背景
	std::shared_ptr<BackGround>back;

	// マップ
	std::shared_ptr<Map>map;

	// プレイヤー
	std::shared_ptr<Player>pl;

	// 敵
	std::shared_ptr<EnemyMane>enemy;
};
