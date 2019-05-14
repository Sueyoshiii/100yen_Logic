#include "Main.h"
#include <MyLib.h>

// �R���X�g���N�^
Main::Main(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

// �f�X�g���N�^
Main::~Main()
{
}

// �`��
void Main::Draw(void)
{
	lib.lock()->Clear();

	lib.lock()->Execution();
}

// ����
void Main::UpData(void)
{
}