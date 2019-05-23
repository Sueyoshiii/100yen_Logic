#pragma once
#include "../Info/Info.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

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

// 定数
typedef
struct ConstParam
{
	static const unsigned int DIV_SIZE;

	static const float SPEED;		// 速度
	static const float DUSH_POW;	// ダッシュ力
	static const float JUMP_POW;	// ジャンプ力
	static const float GR;			// 重力
	static const float GROUND;		// 地面
}Const;

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
	// キャラクターデータ読み込み
	void LoadData(const std::string& filePath);

	// アニメーションの終了を調べる
	bool CheckAnimEnd();

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
	unsigned int frame;

	// アニメーションカウント
	unsigned int animCnt;

	// アニメーション配列インデックス
	unsigned int index;

	// 状態の関数ポインタ
	std::unordered_map<std::string, std::function<void(void)>> func;

	// データ
	std::weak_ptr<std::unordered_map<std::string, Data>> info;
private:
	std::string ConvertToString(const ST state);
	std::unordered_map<ST, std::string> stMap;
};