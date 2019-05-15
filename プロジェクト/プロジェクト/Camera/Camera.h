#pragma once
#include <MyLib.h>
#include <memory>

class Player;

struct Rect
{
	Vec2f pos;
	float w, h;
	Rect() : pos(Vec2f()), w(0.0f), h(0.0f) {};
	Rect(const Vec2f& pos, const float w, const float h) :
		pos(pos), w(w), h(h) {};
	float Top()const { return pos.y - h / 2.0f; };
	float Bottom()const { return pos.y + h / 2.0f; };
	float Left()const { return pos.x - w / 2.0f; };
	float Right()const { return pos.x - w / 2.0f; };
};

class Camera
{
public:
	Camera();
	Camera(const Rect& range);
	~Camera();

	// �X�V
	void Update();

	// 
	Vec2f Correction(const Vec2f& pos);

	// ���W�擾
	Vec2f GetPos()const;

	// �J�����͈͎̔擾
	Rect GetViewPort()const;

	// �Ώۂ�ݒ�
	void SetFocus(std::weak_ptr<Player> pl);
private:
	Rect range;
	Vec2f pos;
	std::weak_ptr<Player> pl;
};