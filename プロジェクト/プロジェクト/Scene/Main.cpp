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
}

// ����
void Main::UpData(void)
{
}