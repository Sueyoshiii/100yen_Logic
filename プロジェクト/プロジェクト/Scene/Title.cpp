#include "Title.h"
#include "GameMain.h"
#include "../Game/Game.h"
#include "../Json/JsonLoader.h"
#include <crtdbg.h>

#include "../Okdio/Okdio.h"
#pragma comment (lib, "Okdio.lib")

// コンストラクタ
Title::Title(std::weak_ptr<MyLib>lib) :
	checkNext(false)
{
	this->lib = lib;

	okmonn::CreateObj(IID_PPV_ARGS(&playMusic));
	playMusic->Load("data/sound/se/general/button.wav");

	img[0].Load("img/Title/Title_Back.png");
	img[0].size.y *= 2.0f;

	img[1].Load("img/Title/Title_Board.png");
	img[1].size.y *= 2.0f;
	img[1].size /= 1.5f;
	img[1].pos -= img[1].size / 2.0f;
	img[1].pos += {
		float(lib.lock()->GetWinSize().x) / 2.0f,
		float(lib.lock()->GetWinSize().y) / 2.0f
	};

	guide.Load("img/Title/Title_Button.png");
	guide.size.y *= 2.0f;
	guide.size /= 1.5f;
	guide.pos -= img[1].size / 2.0f;
	guide.pos += {
		float(lib.lock()->GetWinSize().x) / 2.0f,
		float(lib.lock()->GetWinSize().y) * 1.5f
	};

	alpha = 0.0f;
	guideAlpha = 0.0f;
}

// デストラクタ
Title::~Title()
{
	playMusic->Release();
}
// 描画
void Title::Draw()
{
	for (unsigned int i = 0; i < _countof(img); ++i)
	{
		lib.lock()->Draw(img[i], alpha);
	}
	lib.lock()->Draw(guide, guideAlpha);
}

// 処理
void Title::Update()
{
	for (unsigned int button = unsigned int(Key::LButton); button <= unsigned int(Key::Ome_102); ++button)
	{
		if (Input::Get().IsTrigger(Key(button)))
		{
			checkNext = true;
		}
	}

	alpha = std::fmin(alpha, 1.0f);
	if (checkNext)
	{
		playMusic->Play(false);
		alpha -= 0.02f;
		guideAlpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			Game::Get().ChangeScene(new GameMain(lib));
		}
	}
	else
	{
		alpha += 0.02f;
		static float alphaCnt = 0.02f;
		if (alpha >= 1.0f)
		{
			if (guideAlpha > 1.0f || guideAlpha < 0.0f)
			{
				alphaCnt = -alphaCnt;
			}
			guideAlpha += alphaCnt;
		}
	}
}
