#pragma once
#include "../Typedef.h"
#include "../Info/Info.h"
#include "../Stage/Stage.h"
#include "../Camera/Camera.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <vector>
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

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// ��ԑJ��
	void ChangeState(const ST state);

	// �m�b�N�o�b�N
	void KnockBack(const Vec2f& vec);

	// �Փˋ�`���擾
	std::vector<HitRect<Vec2f>> GetRect();

	// ���W�擾
	Vec2f GetPos()const;

	// �T�C�Y�擾
	Vec2f GetSize()const;

	// ���]�t���O�擾
	bool GetTurnFlag()const;

	// ���G�t���O�擾
	bool GetInvincibleFlag()const;

	// ���]�t���O�Z�b�g
	void SetTurnFlag(const bool flag);
protected:
	// �X�e�[�W���ɍ��W��␳
	void CorrectPosInStage();

	// ����
	void FallUpdate();

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

	// ���[�J�����W�̍X�V
	void UpdateLocalPos();

	// ���G����
	void InvicibleUpdate();

	///*����MyLib�N���X�̎�Q�Ƃ������o�Ƃ��Ď�������Ȃ�A
	///*MyLibChild�N���X���̃C���^�[�t�F�[�X�N���X��p�����ق�����X�y
	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �J������Q��
	std::weak_ptr<Camera> cam;

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

	// �A���t�@�l
	float alpha;

	// ���]�t���O
	bool turnFlag;

	// �o�߃t���[��
	float frame;

	// �A�j���[�V�����J�E���g
	unsigned int animCnt;

	// �A�j���[�V�����z��C���f�b�N�X
	unsigned int index;

	// ��Ԃ̊֐��|�C���^
	std::unordered_map<ST, std::function<void(void)>> func;

	// �f�[�^
	std::weak_ptr<std::unordered_map<std::string, Data>> info;

	// �A�j���[�V�����~�߂�
	bool stopFlag;

	// �̗�
	int hp;

	// �ړ����x
	float speed;

	// �_�b�V����
	float dushPow;

	// �W�����v��
	float jumpPow;

	// ���G�t���O
	bool invincibleFlag;

	// �m�b�N�o�b�N����
	float knockBackRange;
private:
	// ��ԏ�����
	void InitState();

	// ��`
	std::unordered_map<ST, std::vector<std::vector<Primitive>>> box;
};