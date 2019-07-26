#pragma once
#include "../CharaEffect.h"

class Slash :
	public CharaEffect
{
public:
	Slash(const std::string& state, const CharacterType& plType, const Vec2f& pos, const Vec2f& size, const bool turnFlag);
	~Slash();

	// �X�V
	void Update();

	// �`��
	void Draw();
	void Draw(std::weak_ptr<MyLib> lib);

	void SetPos(const Vec2f& pos);
private:
	void InitState();

	void InitType();

	void TypeNormal();
	void TypeWolf();

	// ����
	void FirstAttackUpdate();
	// ��
	void SecondAttackUpdate();
	// �O��
	void ThirdAttackUpdate();
	// �l��
	void FourthAttackUpdate();
	// �U������
	void AttackUpdate(const unsigned int attackInterval);
	// �W�����v�U��
	void JumpAttackUpdate();

	// �U���t���[��
	unsigned int attackCnt;

	// ��ԃR���e�i
	std::unordered_map<std::string, std::string> stateMap;

	// �`�Ԋ֐�
	std::unordered_map<CharacterType, std::function<void()>> typeFunc;
};