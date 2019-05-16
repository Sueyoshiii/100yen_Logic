#pragma once
#include <MyLib.h>
#include <memory>

class Player;

class Camera
{
public:
	Camera();
	~Camera();

	// �X�V
	void Update();

	// 
	Vec2f Correction(const Vec2f& pos);

	// ���W�擾
	Vec2f GetPos()const;

	// �Ώۂ�ݒ�
	void SetFocus(std::weak_ptr<Player> pl);
private:
	Vec2f pos;
	std::weak_ptr<Player> pl;
};