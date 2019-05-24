#pragma once
#include "../Info/Info.h"
#include "../Stage/Stage.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#define In Input::Get()

// ���
typedef
enum class State
{
	Neutral,
	Walk,
	Jump,
	Dash,
	Attack1,
	Attack2,
	Attack3,
	Damage,
	Death
}ST;

// �萔
typedef
struct ConstParam
{
	// ���x
	static const float SPEED;
	// �_�b�V����
	static const float DUSH_POW;
	// �W�����v��
	static const float JUMP_POW;
	// �U���Ԋu
	static const unsigned int ATTACK_INTERVAL;
	// �d��
	static const float GR;
	// �n��
	static const float GROUND;
}Const;

class Character
{
public:
	Character();
	~Character();

protected:
	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �X�e�[�W���ɍ��W��␳
	void CorrectPosInStage();

	// ����
	void FallUpdate();

	// ��ԑJ��
	void ChangeState(const ST state);

	// ���W�擾
	Vec2f GetPos()const;

	// �T�C�Y�擾
	Vec2f GetSize()const;

	// �L�����N�^�[�f�[�^�ǂݍ���
	void LoadData(const std::string& filePath);

	// �L�����N�^�[�摜�ǂݍ���
	void LoadImage(const std::string& filePath);

	// �摜�`��
	void DrawImage();

	// �Փˋ�`�`��
	void DrawRect();

	// �A�j���[�V�����X�V
	void AnimationUpdate();

	// �A�j���[�V�����̏I���𒲂ׂ�
	bool CheckAnimEnd();

	///*����MyLib�N���X�̎�Q�Ƃ������o�Ƃ��Ď�������Ȃ�A
	///*MyLibChild�N���X���̃C���^�[�t�F�[�X�N���X��p�����ق�����X�y
	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �L�����n���h��
	Texture tex;

	// ���
	ST state;

	// 1�O�̏��
	ST oldState;

	// ��ԃR���e�i
	std::unordered_map<ST, std::string> stMap;

	// ���[�J�����W
	Vec2f localPos;

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
	std::unordered_map<ST, std::function<void(void)>> func;

	// �f�[�^
	std::weak_ptr<std::unordered_map<std::string, Data>> info;
private:
	// ��ԏ�����
	void InitState();

	Primitive box;
};