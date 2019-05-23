#pragma once
#include "../Info/Info.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

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
	Death
}ST;

// �萔
typedef
struct ConstParam
{
	static const unsigned int DIV_SIZE;

	static const float SPEED;		// ���x
	static const float DUSH_POW;	// �_�b�V����
	static const float JUMP_POW;	// �W�����v��
	static const float GR;			// �d��
	static const float GROUND;		// �n��
}Const;

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
	void ChangeState(const std::string& state);

	// ���W�擾
	Vec2f GetPos()const;

	// �T�C�Y�擾
	Vec2f GetSize()const;
protected:
	// �L�����N�^�[�f�[�^�ǂݍ���
	void LoadData(const std::string& filePath);

	// �A�j���[�V�����̏I���𒲂ׂ�
	bool CheckAnimEnd();

	///*����MyLib�N���X�̎�Q�Ƃ������o�Ƃ��Ď�������Ȃ�A
	///*MyLibChild�N���X���̃C���^�[�t�F�[�X�N���X��p�����ق�����X�y
	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �L�����n���h��
	Texture tex;

	// ���
	std::string state;

	// ���x
	Vec2f vel;

	// ���]�t���O
	bool turnFlag;

	// �o�߃t���[��
	unsigned int frame;

	// �A�j���[�V�����J�E���g
	unsigned int animCnt;

	// �A�j���[�V�����z��C���f�b�N�X
	unsigned int index;

	// ��Ԃ̊֐��|�C���^
	std::unordered_map<std::string, std::function<void(void)>> func;

	// �f�[�^
	std::weak_ptr<std::unordered_map<std::string, Data>> info;
private:
	std::string ConvertToString(const ST state);
	std::unordered_map<ST, std::string> stMap;
};