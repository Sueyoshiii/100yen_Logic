#pragma once
#include "Enemy.h"

class Wolf : 
	public Enemy
{
public:
	Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos);
	~Wolf();
	
	// XV
	void Update();

	// •`‰æ
	void Draw();
private:
	// ó‘Ô‰Šú‰»
	void InitFunc();
	// ‘Ò‹@
	void NeutralUpdate();
	// •à‚«
	void WalkUpdate();
};