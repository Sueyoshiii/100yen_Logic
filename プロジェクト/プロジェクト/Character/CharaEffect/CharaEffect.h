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

	// 描画
	virtual void Draw(std::weak_ptr<MyLib> lib) = 0;

	// 削除フラグ取得
	bool GetDeleteFlag()const;

	virtual void SetPos(const Vec2f& pos) = 0;
protected:
	// 削除フラグ
	bool deleteFlag;
};
