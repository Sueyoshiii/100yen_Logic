#include "Over.h"
#include <MyLib.h>

// �R���X�g���N�^
Over::Over(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
}

// �f�X�g���N�^
Over::~Over()
{
}

// �`��
void Over::Draw(void)
{
	lib.lock()->Clear();

	lib.lock()->Execution();
}

// ����
void Over::UpData(void)
{
}
