#include "Over.h"
#include "Title.h"
#include "../Game/Game.h"
#include <algorithm>

#include "../Okdio/Okdio.h"
#include "../SoundManager.h"
#pragma comment (lib, "Okdio.lib")

// コンストラクタ
Over::Over(std::weak_ptr<MyLib> lib) :
	checkNext(false)
{
	this->lib = lib;

	tex.Load("img/deadend/deadend_moji.png");
	tex.size.y *= 2.0f;
	tex.pos -= tex.size / 2.0f;
	tex.pos += {
		float(lib.lock()->GetWinSize().x) / 2.0f,
		float(lib.lock()->GetWinSize().y)
	};

	alpha = 0.0f;

	SoundManager::Get().Stop(0);
	SoundManager::Get().Stop(1);
}

// デストラクタ
Over::~Over()
{
}

// 描画
void Over::Draw()
{
	lib.lock()->Draw(tex, alpha);
}

// 処理
void Over::Update()
{
	if (Input::Get().IsTrigger(Key::Return))
	{
		checkNext = true;
	}

	alpha = std::fmin(alpha, 1.0f);
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
