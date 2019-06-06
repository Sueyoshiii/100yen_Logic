#pragma once
#include "Enemy.h"

class Wolf : 
	public Enemy
{
public:
	Wolf(std::weak_ptr<MyLib> lib, std::weak_ptr<Player> pl, std::weak_ptr<Camera> cam, const Vec2f& pos);
	~Wolf();
	
	// �X�V
	void Update();

	// �`��
	void Draw();
private:
	// ��ԏ�����
	void InitFunc();
	// �ҋ@
	void NeutralUpdate();
	// �p�j
	void Loitering();
	// ����
	void Alert();
	// ����
	void WalkUpdate();
	void Walk();
	// �U��
	void AttackUpdate();
	void Attack();
	// ��_���[�W
	void DamageUpdate();
	// ���S
	void DeathUpdate();

	// �x�_
	Vec2f fulcrum;

	// �N�[���^�C��
	unsigned int coolTime;
};