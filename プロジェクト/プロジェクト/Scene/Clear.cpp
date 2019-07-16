#include "Clear.h"
#include "Title.h"
#include "../Game/Game.h"
#include <algorithm>
#include <MyLib.h>

// コンストラクタ
Clear::Clear(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

	tex.Load("img/clear.png");
	tex.pos -= tex.size / 2.0f;
	tex.pos += {
		float(lib.lock()->GetWinSize().x) / 2.0f,
		float(lib.lock()->GetWinSize().y)
	};

	alpha = 0.0f;
}

// デストラクタ
Clear::~Clear()
{
}

// 描画
void Clear::Draw()
{
	lib.lock()->Draw(tex, alpha);
}

// 処理
void Clear::Update()
{
	if (Input::Get().IsTrigger(Key::Return))
	{
		checkNext = true;
	}

	alpha = std::min(alpha, 1.0f);
	if (checkNext)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			Game::Get().ChangeScene(new Title(lib));
		}
	}
	else
	{
		alpha += 0.02f;
	}
}
