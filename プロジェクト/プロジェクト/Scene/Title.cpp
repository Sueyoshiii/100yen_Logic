#include "Title.h"
#include "GameMain.h"
#include "../Game/Game.h"
#include "../Json/JsonLoader.h"
//#include <Okdio.h>
#include <crtdbg.h>
//
//#pragma comment(lib, "Okdio.lib")

// �R���X�g���N�^
Title::Title(std::weak_ptr<MyLib>lib) :
	checkNext(false)
{
	this->lib = lib;

	tex.Load("img/title.png");
	tex.pos -= tex.size / 2.0f;
	tex.pos += {
		float(lib.lock()->GetWinSize().x) / 2.0f,
		float(lib.lock()->GetWinSize().y)
	};

	alpha = 0.0f;

	//auto hr = okmonn::CreateObj(IID_PPV_ARGS(&okdio));
	//_ASSERT(hr == S_OK);
	//hr = okdio->Load("bgm/INSIDE.wav");
	//_ASSERT(hr == S_OK);
	//hr = okdio->Play(true);
	//_ASSERT(hr == S_OK);
}

// �f�X�g���N�^
Title::~Title()
{
	//okdio->Release();
}
// �`��
void Title::Draw()
{
	lib.lock()->Draw(tex, alpha);
}

// ����
void Title::Update()
{
	for (unsigned int button = 1; button <= unsigned int(Key::Ome_102); ++button)
	{
		if (Input::Get().IsTrigger(Key(button)))
		{
			checkNext = true;
		}
	}

	alpha = std::fmin(alpha, 1.0f);
	if (checkNext)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			Game::Get().ChangeScene(new GameMain(lib));
		}
	}
	else
	{
		alpha += 0.02f;
	}
}
