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

	// 描画
	virtual void Draw(std::weak_ptr<MyLib> lib) = 0;

	// 削除
	virtual void Delete() = 0;

	// 削除フラグ取得
	bool GetDeleteFlag()const;
protected:
	// 削除フラグ
	bool deleteFlag;
};
