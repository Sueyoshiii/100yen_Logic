#pragma once
#include "../CharaEffect.h"

class Slash :
	public CharaEffect
{
public:
	Slash(const std::string& state, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Slash();

	// �X�V
	void Update();

	// �`��
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	// �폜
	void Delete();
private:
	void InitState();

	// ����
	void FirstAttackUpdate();
	// ��
	void SecondAttackUpdate();
	// �O��
	void ThirdAttackUpdate();
	// �U������
	void AttackUpdate(const unsigned int attackInterval);

	// �U���t���[��
	unsigned int attackCnt;

	// ��ԃR���e�i
	std::unordered_map<std::string, std::string> stateMap;
};