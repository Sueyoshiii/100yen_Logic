#pragma once
#include "../CharaEffect.h"

class Player;

enum class Status
{
	Drop,		// ドロップ
	Suction		// 吸収
};

class Flower :
	public CharaEffect
{
public:
	Flower(const Vec2f& pos, std::weak_ptr<Player> pl);
	~Flower();

	// 更新
	void Update();

	// 描画
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	// 削除
	void Delete();
private:
	// ドロップ
	void DropUpdate();

	// 吸収
	void SuctionUpdate();

	std::vector<std::string> path;
	std::vector<Texture> texs;
	Status st;
	std::unordered_map<Status, std::function<void(void)>> stFunc;
	std::weak_ptr<Player> pl;
};
