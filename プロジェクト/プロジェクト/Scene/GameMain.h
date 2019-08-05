#pragma once
#include "Scene.h"
#include <memory>

class Camera;
class BackGround;
class Player;
class Stage;
class Okdio;

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
	void Update();
private:
	// シーン切り替え
	void ChangeNextScene(Scene* scene);

	// ルーム切り替え
	void CheckChangeRoom();

	// 登録されているオブジェクトの解放
	void DeleteObject();

	// カメラ
	std::shared_ptr<Camera> cam;

	// 背景
	std::shared_ptr<BackGround> bg;

	// プレイヤー
	std::shared_ptr<Player> pl;
};
