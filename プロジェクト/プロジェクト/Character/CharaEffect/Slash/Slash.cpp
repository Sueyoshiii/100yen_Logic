#include "Slash.h"
#include "../../Player/Player.h"

// �R���X�g���N�^
Slash::Slash(std::weak_ptr<Player> pl)
{
	LoadData("");
	LoadImage("player_effect.png");
}

// �f�X�g���N�^
Slash::~Slash()
{
}

// �X�V
void Slash::Update()
{
}

// �`��
void Slash::Draw()
{
}
void Slash::Draw(std::weak_ptr<MyLib> lib)
{
}
