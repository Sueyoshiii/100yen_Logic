#pragma once
#include <memory>

class Application;
class Scene;

class GameMane
{
public:
	// デストラクタ
	~GameMane();

	// インスタンス変数の取得
	static GameMane& Get(void) {
		static GameMane instance;
		return instance;
	}

	// シーンの移行
	void ChangeScene(Scene* scene);

	// 処理
	void Run(void);

private:
	// コンストラクタ
	GameMane();
	GameMane(const GameMane&) = delete;
	void operator=(const GameMane&) = delete;

	// 初期化
	void Init(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	// 終了
	void End(void);


	// アプリケーション
	std::shared_ptr<Application>app;

	// シーン
	std::shared_ptr<Scene>scene;
};
