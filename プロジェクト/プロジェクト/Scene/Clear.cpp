#include "Clear.h"
#include <MyLib.h>

// �R���X�g���N�^
Clear::Clear(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

// �f�X�g���N�^
Clear::~Clear()
{
}

// �`��
void Clear::Draw(void)
{
	lib.lock()->Clear();

	lib.lock()->Execution();
}

// ����
void Clear::UpData(void)
{
}
