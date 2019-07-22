#pragma once
#include "../Character.h"

class Player;

enum class EffectType
{
	Flower,
	Slashing,
	BloodSplash
};

class CharaEffect :
	public Character
{
public:
	CharaEffect();
	virtual ~CharaEffect();

	// �`��
	virtual void Draw(std::weak_ptr<MyLib> lib) = 0;

	// �폜�t���O�擾
	bool GetDeleteFlag()const;

	virtual void SetPos(const Vec2f& pos) = 0;
protected:
	// �폜�t���O
	bool deleteFlag;
};
