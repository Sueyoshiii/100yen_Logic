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
	virtual void Draw(std::weak_ptr<MyLib> lib) = 0;
	virtual void Delete() = 0;

	bool GetDeleteFlag()const;
protected:
	bool deleteFlag;
};
