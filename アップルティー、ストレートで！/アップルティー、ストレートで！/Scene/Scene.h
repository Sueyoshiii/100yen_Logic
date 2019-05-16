#pragma once
#include <string>
#include <memory>
#include <unordered_map>

template<typename T>
class Vector2;
typedef Vector2<float> Vec2f;
class Application;

class Scene
{
public:
	// コンストラクタ
	Scene();
	// デストラクタ
	virtual ~Scene();

	// 描画
	virtual void Draw(void) = 0;

	// 処理
	virtual void UpData(void) = 0;

protected:
	// 画像の読み込み
	void LoadImg(const std::string& name, const std::string& fileName);

	// 画像の描画
	void DrawImg(const std::string& name, const Vec2f& pos, const Vec2f& size, const Vec2f& uvPos, const Vec2f& uvSize,
		const float& alpha = 1.0f, const bool& turnX = false, const bool& turnY = false);

	// 画像の削除
	void Delete(const std::string& name);
	void Delete(void);


	// アプリケーション
	std::weak_ptr<Application>app;

	// 画像ハンドル
	std::unordered_map<std::string, int>image;
};
