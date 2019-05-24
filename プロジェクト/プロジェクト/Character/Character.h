#pragma once
#include "../Info/Info.h"
#include "../Stage/Stage.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

#define In Input::Get()

// 状態
typedef
enum class State
{
	Neutral,
	Walk,
	Jump,
	Dash,
	Attack1,
	Attack2,
	Attack3,
	Damage,
	Death
}ST;

// 定数
typedef
struct ConstParam
{
	// 速度
	static const float SPEED;
	// ダッシュ力
	static const float DUSH_POW;
	// ジャンプ力
	static const float JUMP_POW;
	// 攻撃間隔
	static const unsigned int ATTACK_INTERVAL;
	// 重力
	static const float GR;
	// 地面
	static const float GROUND;
}Const;

class Character
{
public:
	Character();
	~Character();

protected:
	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// ステージ内に座標を補正
	void CorrectPosInStage();

	// 落下
	void FallUpdate();

	// 状態遷移
	void ChangeState(const ST state);

	// 座標取得
	Vec2f GetPos()const;

	// サイズ取得
	Vec2f GetSize()const;

	// キャラクターデータ読み込み
	void LoadData(const std::string& filePath);

	// キャラクター画像読み込み
	void LoadImage(const std::string& filePath);

	// 画像描画
	void DrawImage();

	// 衝突矩形描画
	void DrawRect();

	// アニメーション更新
	void AnimationUpdate();

	// アニメーションの終了を調べる
	bool CheckAnimEnd();

	///*もしMyLibクラスの弱参照をメンバとして持たせるなら、
	///*MyLibChildクラス等のインターフェースクラスを用いたほうが後々楽
	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// キャラハンドル
	Texture tex;

	// 状態
	ST state;

	// 1つ前の状態
	ST oldState;

	// 状態コンテナ
	std::unordered_map<ST, std::string> stMap;

	// ローカル座標
	Vec2f localPos;

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
	std::unordered_map<ST, std::function<void(void)>> func;

	// データ
	std::weak_ptr<std::unordered_map<std::string, Data>> info;
private:
	// 状態初期化
	void InitState();

	Primitive box;
};