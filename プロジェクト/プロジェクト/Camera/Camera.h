#pragma once
#include <MyLib.h>
#include <memory>

class Player;

class Camera
{
public:
	Camera(std::weak_ptr<MyLib> lib);
	~Camera();

	// �X�V
	void Update();

	// 
	Vec2f Correction(const Vec2f& pos);

	// ���W�擾
	Vec2f GetPos()const;

	// �Ώۂ�ݒ�
	void SetFocus(std::weak_ptr<Player> pl);

	// �U���t���O��ݒ�
	void SetVibrationFlag(const bool flag);
private:
	// ���W
	Vec2f pos;

	// �T�C�Y
	Vec2f size;

	// ���C�u����
	std::weak_ptr<MyLib> lib;

	// �v���C���[
	std::weak_ptr<Player> pl;

	// �U���t���O
	bool vibrationFlag;

	Vec2f fulcrum;

	float speed;

	unsigned int vibrationCnt;
};