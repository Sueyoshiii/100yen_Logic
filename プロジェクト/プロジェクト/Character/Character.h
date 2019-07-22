#pragma once
#include "../Typedef.h"
#include "../Info/Info.h"
#include "../Stage/StageManager.h"
#include "../Camera/Camera.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

#define INPUT Input::Get()

enum class CharacterType
{
	// �v���C���[
	PL_NORMAL,
	PL_WOLF,
	// �G
	EM_WEAK_WOLF,
	// �G�t�F�N�g
	EF_PL_NORMAL_SLASH,
	EF_PL_WOLF_SLASH,
	EF_BLOOD_SPLASH
};

// �L�����N�^�[�̃p�����[�^
struct CharacterParameter
{
	// �̗�
	int hp;

	// �ړ����x
	float speed;

	// �U����
	int attackPow;

	// �h���
	int defensePow;

	// �_�b�V����
	float dashPow;

	// �W�����v��
	float jumpPow;

	CharacterParameter() :
		hp(0), speed(0.0f), attackPow(0), defensePow(0), 
		dashPow(0.0f), jumpPow(0.0f) {}

	CharacterParameter(const int hp, const float speed, const int attackPow,
		const int defencePow, const float dashPow, const float jumpPow) :
		hp(hp), speed(speed), attackPow(attackPow), defensePow(defencePow), 
		dashPow(dashPow), jumpPow(jumpPow) {}

	CharacterParameter(const float speed, const int attackPow,
		const int defencePow, const float dashPow, const float jumpPow) :
		speed(speed), attackPow(attackPow), defensePow(defencePow), 
		dashPow(dashPow), jumpPow(jumpPow) {}
};

class Character
{
public:
	Character();
	virtual ~Character();

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// ��ԑJ��
	void ChangeState(const std::string& state);

	// �m�b�N�o�b�N
	void KnockBack(const Vec2f& vec);

	// �������擾
	bool CheckAlive();

	// �Փˋ�`���擾
	std::vector<HitRect<Vec2f>> GetRect();

	// ���W�擾
	Vec2f GetPos();

	// �T�C�Y�擾
	Vec2f GetSize();

	// ���]�t���O�擾
	bool GetTurnFlag()const;

	// ���G�t���O�擾
	bool GetInvincibleFlag()const;

	// �X�e�[�^�X�擾
	std::string GetState()const;

	// �p�����[�^�擾
	CharacterParameter GetParam()const;

	// �����擾
	Vec2f GetFootPos();

	// ���]�t���O�Z�b�g
	void SetTurnFlag(const bool flag);

	// �_���[�W�Z�b�g
	void SetDamage(const int attackPow, const int defensePow);
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
	bool CheckAnimEnd(const unsigned int num = 1);

	// ���[�J�����W�̍X�V
	void UpdateLocalPos();

	// ���G����
	void InvicibleUpdate();

	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �J������Q��
	std::weak_ptr<Camera> cam;

	// �L�����n���h��
	std::unordered_map<CharacterType, Texture> tex;

	// ���
	std::string state;

	// 1�O�̏��
	std::string oldState;

	// ���[���h���W
	Vec2f worldPos;

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
	std::unordered_map<std::string, std::function<void(void)>> func;

	// �f�[�^
	std::unordered_map<CharacterType, std::weak_ptr<std::unordered_map<std::string, Data>>> info;

	// �A�j���[�V�����~�߂�
	bool stopFlag;

	// �L�����N�^�[�p�����[�^
	CharacterParameter cParam;

	// ���G�t���O
	bool invincibleFlag;

	// �m�b�N�o�b�N����
	float knockBackRange;

	// �_���[�W
	int damage;

	// �W�����v�t���O
	bool jumpFlag;

	// �L�����^�C�v
	CharacterType type;

	// �O�̃L�����^�C�v
	CharacterType oldType;
private:
	// ��`
	std::unordered_map<std::string, std::vector<std::vector<Primitive>>> box;
};