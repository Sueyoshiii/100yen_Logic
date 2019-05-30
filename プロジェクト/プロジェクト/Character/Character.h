#pragma once
#include "../Typedef.h"
#include "../Info/Info.h"
#include "../Stage/Stage.h"
#include "../Camera/Camera.h"
#include <MyLib.h>
#include <memory>
#include <string>
#include <vector>
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

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 状態遷移
	void ChangeState(const ST state);

	// ノックバック
	void KnockBack(const Vec2f& vec);

	// 衝突矩形を取得
	std::vector<HitRect<Vec2f>> GetRect();

	// 座標取得
	Vec2f GetPos()const;

	// サイズ取得
	Vec2f GetSize()const;

	// 反転フラグ取得
	bool GetTurnFlag()const;

	// 無敵フラグ取得
	bool GetInvincibleFlag()const;

	// 反転フラグセット
	void SetTurnFlag(const bool flag);
protected:
	// ステージ内に座標を補正
	void CorrectPosInStage();

	// 落下
	void FallUpdate();

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

	// ローカル座標の更新
	void UpdateLocalPos();

	// 無敵処理
	void InvicibleUpdate();

	///*もしMyLibクラスの弱参照をメンバとして持たせるなら、
	///*MyLibChildクラス等のインターフェースクラスを用いたほうが後々楽
	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// カメラ弱参照
	std::weak_ptr<Camera> cam;

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

	// アルファ値
	float alpha;

	// 反転フラグ
	bool turnFlag;

	// 経過フレーム
	float frame;

	// アニメーションカウント
	unsigned int animCnt;

	// アニメーション配列インデックス
	unsigned int index;

	// 状態の関数ポインタ
	std::unordered_map<ST, std::function<void(void)>> func;

	// データ
	std::weak_ptr<std::unordered_map<std::string, Data>> info;

	// アニメーション止める
	bool stopFlag;

	// 体力
	int hp;

	// 移動速度
	float speed;

	// ダッシュ力
	float dushPow;

	// ジャンプ力
	float jumpPow;

	// 無敵フラグ
	bool invincibleFlag;

	// ノックバック距離
	float knockBackRange;
private:
	// 状態初期化
	void InitState();

	// 矩形
	std::unordered_map<ST, std::vector<std::vector<Primitive>>> box;
};