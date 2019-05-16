#include "EnemyMane.h"
#include "Player.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "../Application/Application.h"

// コンストラクタ
EnemyMane::EnemyMane(std::weak_ptr<Application>app, std::weak_ptr<Camera>cam, std::weak_ptr<Player>pl) : app(app), cam(cam), pl(pl)
{
	enemy.clear();

	Init();
}

// デストラクタ
EnemyMane::~EnemyMane()
{
}

// 初期化
void EnemyMane::Init(void)
{
	func.clear();

	func["1"] = [&](std::list<std::shared_ptr<Enemy>>& list, std::weak_ptr<Application>app,std::weak_ptr<Camera>cam, std::weak_ptr<Player>pl, const Vec2f& pos, const Vec2f& size)->void {
		list.push_back(std::make_shared<Enemy1>(app,cam, pl, pos, size));
	};

	func["2"] = [&](std::list<std::shared_ptr<Enemy>>& list, std::weak_ptr<Application>app, std::weak_ptr<Camera>cam, std::weak_ptr<Player>pl, const Vec2f& pos, const Vec2f& size)->void {
		list.push_back(std::make_shared<Enemy2>(app, cam, pl, pos, size));
	};

}

// 敵の生成
void EnemyMane::Create(const std::string & type, const Vec2f & pos, const Vec2f & size)
{
	func[type](enemy, app, cam, pl, pos, size);
}

// 描画
void EnemyMane::Draw(void)
{
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

// 処理
void EnemyMane::UpData(void)
{
	for (auto itr = enemy.begin(); itr != enemy.end();)
	{
		(*itr)->UpData();
		if ((*itr)->GetDead())
		{
			itr = enemy.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

// クリア
void EnemyMane::Clear(void)
{
	enemy.clear();
}
