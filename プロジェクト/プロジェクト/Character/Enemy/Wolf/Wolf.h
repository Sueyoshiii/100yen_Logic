#pragma once
#include "../Enemy.h"

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
	// ����
	void WalkUpdate();
	void CheckWalk();
	// ���K
	void HowlingUpdate();
	void CheckHowling();
	// �Њd
	void ThreatUpdate();
	void CheckTreat();
	// �ǂ�������
	void RunUpdate();
	void CheckRun();
	// ����
	void SaveUpdate();
	void CheckSave();
	// �U��
	void AttackUpdate();
	void CheckAttack();
	// ��_���[�W
	void DamageUpdate();
	// ���S
	void DeathUpdate();

	// �x�_
	Vec2f fulcrum;

	// �N�[���^�C��
	unsigned int coolTime;

	// �����t���O
	bool discovery;

	unsigned int cnt;

	bool coolFlag;
};