#pragma once
#include "../Character.h"

class Player;

enum class EffectType
{
	Flower,
	Slashing
};

class CharaEffect :
	public Character
{
public:
	CharaEffect();
	~CharaEffect();

	// �`��
	virtual void Draw(std::weak_ptr<MyLib> lib) = 0;

	// �폜
	virtual void Delete() = 0;

	// �폜�t���O�擾
	bool GetDeleteFlag()const;
protected:
	// �폜�t���O
	bool deleteFlag;
};
