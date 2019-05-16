#include "Map.h"
#include "../Application/Application.h"
#include <random>
#include <algorithm>

// コンストラクタ
Map::Map(std::weak_ptr<Application> app) : app(app),
	size({300.0f, 100.0f}), mass(0), ground(0.0f), read(0)
{
	id.clear();

	Init();
}

// デストラクタ
Map::~Map()
{
}

//	初期化
void Map::Init(void)
{
	mass = static_cast<int>(app.lock()->GetWinSize().x / size.x);
	mass.x *= 4;
	ground = app.lock()->GetWinSize().y - size.y * mass.y;

	id.resize(mass.x * mass.y);
	std::random_device dev;
	std::mt19937 mt(dev());
	std::uniform_int_distribution<int>dist(0, 5);
	std::for_each(id.begin(), id.end(), [&](int& id)->void {
		id = dist(mt);
	});
}

// マップデータの取得
std::vector<int> Map::GetData(const Vec2f & pos)
{
	int index = pos.x / size.x;
	if (read >= index)
	{
		return std::vector<int>();
	}

	std::vector<int>tmp;
	for (int i = read; i < index; ++i)
	{
		for (int n = 0; n < mass.y; ++n)
		{
			tmp.push_back(id[mass.x * n + i]);
		}
	}

	read = index;

	return tmp;
}
