#include "Clear.h"
#include <MyLib.h>

// �R���X�g���N�^
Clear::Clear(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;
#ifdef _DEBUG
	std::cout << "GameClear Scene" << std::endl;
#endif
}

// �f�X�g���N�^
Clear::~Clear()
{
}

// �`��
void Clear::Draw()
{
}

// ����
void Clear::UpData()
{
}
