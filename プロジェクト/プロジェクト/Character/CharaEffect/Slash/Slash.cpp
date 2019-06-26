#include "Slash.h"
#include "../../Player/Player.h"

// コンストラクタ
Slash::Slash(std::weak_ptr<Player> pl)
{
	LoadData("");
	LoadImage("player_effect.png");
}

// デストラクタ
Slash::~Slash()
{
}

// 更新
void Slash::Update()
{
}

// 描画
void Slash::Draw()
{
}
void Slash::Draw(std::weak_ptr<MyLib> lib)
{
}
