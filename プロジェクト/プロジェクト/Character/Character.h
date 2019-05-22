#pragma once
#include <MyLib.h>
#include <memory>
#include <string>
#include <unordered_map>

#define In Input::Get()

// 状態
typedef
enum class State
{
	Neutral,
	Walk,
	Attack1,
	Attack2,
	Attack3,
	Jump,
	Dash,
	Damage,
	Death
}ST;

class Character
{
public:
	Character();
	~Character();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 状態遷移
	void ChangeState(const std::string& state);

	// 座標取得
	Vec2f GetPos()const;

	// サイズ取得
	Vec2f GetSize()const;
protected:
	void LoadData(const std::string& filePath);

	///*もしMyLibクラスの弱参照をメンバとして持たせるなら、
	///*MyLibChildクラス等のインターフェースクラスを用いたほうが後々楽
	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// キャラハンドル
	Texture tex;

	// 状態
	std::string state;

	// 速度
	Vec2f vel;

	// 反転フラグ
	bool turnFlag;

	// 経過フレーム
	unsigned int flame;
private:
	std::string ConvertToString(const ST state);
	std::unordered_map<ST, std::string> stMap;
};