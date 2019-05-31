#pragma once
#include "Character.h"
#include "Player.h"

// �G����B�̂����O
typedef
enum class Enemies
{
	Wolf
}EM;

// �����\����
struct Segment
{
	// �n�_
	Vec2f begin;
	// �I�_
	Vec2f end;
	// �����x�N�g��
	Vec2f vec;

	Segment() : begin(Vec2f()), end(Vec2f()), vec(Vec2f()) {};
	Segment(const Vec2f& begin, const Vec2f& end, const Vec2f& vec) :
		begin(begin), end(end), vec(vec) {};
	Segment(const Vec2f& begin, const Vec2f& end) :
		begin(begin), end(end), vec(end - begin) {};
};

class Enemy : 
	public Character
{
public:
	Enemy();
	~Enemy();
protected:
	virtual void Walk() = 0;

	// �Փ˔���
	void CheckHit();

	// ���E����
	void CheckView();

	// �������m�̏Փ˔���
	bool CheckColSegments(const Segment& seg1, const Segment& seg2);

	// ���E�`��
	void DrawViewRange();

	// ��ԏ�����
	virtual void InitFunc() = 0;

	// �v���C���[
	std::weak_ptr<Player> pl;

	// ���E
	Vec2f viewRange;

	// ���E�͈�
	Primitive viewBox;

	// ���E
	Primitive viewLine;
	Primitive viewLine2;

	// ���E����
	Segment seg;
};