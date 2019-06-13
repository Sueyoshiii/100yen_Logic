#include "Title.h"
#include "GameMain.h"
#include "../Game/Game.h"

// �R���X�g���N�^
Title::Title(std::weak_ptr<MyLib>lib)
{
	this->lib = lib;
#ifdef _DEBUG
	std::cout << "Title Scene" << std::endl;
#endif
}

// �f�X�g���N�^
Title::~Title()
{
}
// �`��
void Title::Draw()
{
}

// ����
void Title::UpData()
{
	if (Input::Get().IsTrigger(Key::Space))
	{
		Game::Get().ChangeScene(new GameMain(lib));
	}
}
