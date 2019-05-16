#pragma once
#include "../etc/Vector2.h"
#include <list>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

class Application;
class Camera;
class Player;
class Enemy;

class EnemyMane
{
public:
	// コンストラクタ
	EnemyMane(std::weak_ptr<Application>app, std::weak_ptr<Camera>cam, std::weak_ptr<Player>pl);
	// デストラクタ
	~EnemyMane();

	// 敵の生成
	void Create(const std::string& type, const Vec2f& pos, const Vec2f& size);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// リストのクリア
	void Clear(void);

private:
	// 初期化
	void Init(void);


	// アプリケーション
	std::weak_ptr<Application>app;

	// カメラ
	std::weak_ptr<Camera>cam;

	// プレイヤー
	std::weak_ptr<Player>pl;

	// 敵
	std::list<std::shared_ptr<Enemy>>enemy;

	// 関数ポインタ
	std::unordered_map<std::string, std::function<void(std::list<std::shared_ptr<Enemy>>&, std::weak_ptr<Application>app,std::weak_ptr<Camera>cam, std::weak_ptr<Player>pl, const Vec2f&, const Vec2f&)>>func;
};
