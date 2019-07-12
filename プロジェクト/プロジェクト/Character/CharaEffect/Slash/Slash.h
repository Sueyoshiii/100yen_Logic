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
	// �U������
	void AttackUpdate(const unsigned int attackInterval);

	// �U���t���[��
	unsigned int attackCnt;

	// ��ԃR���e�i
	std::unordered_map<std::string, std::string> stateMap;

	std::unordered_map<CharacterType, std::function<void()>> typeFunc;
};