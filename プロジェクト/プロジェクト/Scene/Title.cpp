#include "Title.h"
#include <MyLib.h>

// �R���X�g���N�^
Title::Title(std::weak_ptr<MyLib>lib)
{
	this->lib = lib;
}

// �f�X�g���N�^
Title::~Title()
{
}

// �`��
void Title::Draw(void)
{
	lib.lock()->Clear();

	lib.lock()->Execution();
}

// ����
void Title::UpData(void)
{
}
