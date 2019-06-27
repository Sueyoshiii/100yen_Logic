#pragma once
#include "../CharaEffect.h"

class Player;

enum class Status
{
	Drop,		// �h���b�v
	Suction		// �z��
};

class Flower :
	public CharaEffect
{
public:
	Flower(const Vec2f& pos, std::weak_ptr<Player> pl);
	~Flower();

	// �X�V
	void Update();

	// �`��
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	// �폜
	void Delete();
private:
	// �h���b�v
	void DropUpdate();

	// �z��
	void SuctionUpdate();

	std::vector<std::string> path;
	std::vector<Texture> texs;
	Status st;
	std::unordered_map<Status, std::function<void(void)>> stFunc;
	std::weak_ptr<Player> pl;
};
