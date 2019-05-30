#pragma once
#include "Character.h"
#include "Player.h"

// �G����B
typedef
enum class Enemies
{
	Wolf
}EM;

class Enemy : 
	public Character
{
public:
	Enemy();
	~Enemy();
protected:
	// �Փ˔���
	void CheckHit();

	// ���E�`��
	void DrawViewRange();

	// ��ԏ�����
	virtual void InitFunc() = 0;

	// �v���C���[
	std::weak_ptr<Player> pl;

	// ���E
	Vec2f viewRange;

	// ���E�͈�
	Primitive viewBox;
};