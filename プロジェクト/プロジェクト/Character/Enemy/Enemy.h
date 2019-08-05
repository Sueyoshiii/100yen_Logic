#pragma once
#include "../Character.h"
#include "../Player/Player.h"
#include "../CharaEffect/Slash/Slash.h"
#include <array>

// �G����B�̂����O
enum class Enemies
{
	Wolf,
	BOSS_WOLF
};

// ��`�\����
struct Box
{
	// ������W
	Vec2f pos;
	// �T�C�Y
	Vec2f size;
	// �����̃T�C�Y
	Vec2f half;
	// ���S���W
	Vec2f centor;

	Box() : pos(Vec2f()), size(Vec2f()), centor(Vec2f()) {}
	Box(const Vec2f& pos, const Vec2f& size) 
	{
		this->pos = pos;
		this->size = size;
		half = size / 2.0f;
		centor = pos + half;
	}
};

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
	virtual ~Enemy();

	// �폜�t���O�擾
	bool GetDeleteFlag()const;
protected:
	// �Փ˔���
	void CheckHit();

	void CheckHitEffect();

	// ���E����
	bool CheckView();

	// ��`���m�̏Փ˔���
	bool CheckColBox(const Box& box1, const Box& box2);
	bool CheckColBox(const Vec2f& pos1, const Vec2f& size1, const Vec2f& pos2, const Vec2f& size2);

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

	// Player�̋�`����
	std::array<Segment, 4> pSeg;

	// Player�̑O���W
	Vec2f oldPlPos;

	// �폜�t���O
	bool deleteFlag;

	// �C��t���O
	bool stunFlag;

	// �q�b�g��
	unsigned int hitCnt;
private:
};