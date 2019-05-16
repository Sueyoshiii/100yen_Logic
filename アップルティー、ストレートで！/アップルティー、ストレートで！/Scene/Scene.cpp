#include "Scene.h"
#include "../Application/Application.h"

// コンストラクタ
Scene::Scene()
{
}

// デストラクタ
Scene::~Scene()
{
}

// 画像の読み込み
void Scene::LoadImg(const std::string & name, const std::string & fileName)
{
	if (image.find(name) != image.end())
	{
		return;
	}

	app.lock()->LoadTex(image[name], fileName);
}

// 画像の描画
void Scene::DrawImg(const std::string & name, const Vec2f & pos, const Vec2f & size, const Vec2f & uvPos, const Vec2f & uvSize, const float & alpha, const bool & turnX, const bool & turnY)
{
	if (image.find(name) == image.end())
	{
		return;
	}

	app.lock()->DrawTex(image[name], pos, size, uvPos, uvSize, alpha, turnX, turnY);
}

// 画像の削除
void Scene::Delete(const std::string & name)
{
	if (image.find(name) != image.end())
	{
		app.lock()->DeleteTex(image[name]);
		image.erase(image.find(name));
	}
}

// 画像の削除
void Scene::Delete(void)
{
	for (auto itr = image.begin(); itr != image.end(); ++itr)
	{
		app.lock()->DeleteTex(itr->second);
	}
}
