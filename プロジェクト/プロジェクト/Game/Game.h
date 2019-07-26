#pragma once
#include <MyLib.h>
#include <memory>

#pragma comment(lib, "Lib.lib")

class Scene;

class Game
{
public:
	~Game();
	static Game& Get();

	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

	void Finish(void);

	// シーン切り替え
	void ChangeScene(Scene* scene);
private:
	Game();
	Game(const Game&) = delete;
	void operator=(const Game&) = delete;
	
	// ライブラリ
	std::shared_ptr<MyLib> lib;

	// シーン
	std::shared_ptr<Scene> scene;
};
