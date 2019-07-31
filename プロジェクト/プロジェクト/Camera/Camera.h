#pragma once
#include <MyLib.h>
#include <memory>

class Player;

struct ViewRange
{
	float left;
	float right;
	float top;
	float bottom;

	ViewRange() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) {}
	ViewRange(const Vec2f& pos, const Vec2f& size)
	{
		left  = pos.x - size.x/* / 2.0f*/;
		right = pos.x + size.x/* / 2.0f*/;
		top = pos.y - size.y;
		bottom = pos.y + size.y;
	}
	
	float GetLeft()const { return left; }
	float GetRight()const { return right; }
	float GetTop()const { return top; }
	float GetBottom()const { return bottom; }
};

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

	ViewRange GetRange()const;

	// �Ώۂ�ݒ�
	void SetFocus(std::weak_ptr<Player> pl);

	// �U���t���O��ݒ�
	void SetVibrationFlag(const bool flag);

	// ���W�̐ݒ�
	void SetPos(const Vec2f& pos);
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