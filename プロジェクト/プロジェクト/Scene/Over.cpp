#include "Over.h"
#include <MyLib.h>

// �R���X�g���N�^
Over::Over(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
#ifdef _DEBUG
	std::cout << "GameOver Scene" << std::endl;
#endif
}

// �f�X�g���N�^
Over::~Over()
{
}

// �`��
void Over::Draw()
{
}

// ����
void Over::UpData()
{
}
