#pragma once
#include <MyLib.h>
#include <memory>
#include <string>

#define In Input::Get()

// ���
typedef
enum class State
{
	Neutral,
	Walk,
	Attack1,
	Attack2,
	Attack3,
	Jump,
	Dash,
	Damage,
	Deth
}ST;

class Character
{
public:
	Character();
	~Character();

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// ��ԑJ��
	void ChangeState(const ST& state);

	// ���W�擾
	Vec2f GetPos()const;

	// �T�C�Y�擾
	Vec2f GetSize()const;
protected:

	///*����MyLib�N���X�̎�Q�Ƃ������o�Ƃ��Ď�������Ȃ�A
	///*MyLibChild�N���X���̃C���^�[�t�F�[�X�N���X��p�����ق�����X�y
	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �L�����n���h��
	Texture tex;

	// ���
	ST state;

	// ���x
	Vec2f vel;

	// ���]�t���O
	bool turnFlag;

	// �o�߃t���[��
	unsigned int flame;
};