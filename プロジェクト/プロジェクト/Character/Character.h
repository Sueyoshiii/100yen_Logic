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

#define INPUT Input::Get()

// キャラクターの状態
enum class CharacterState
{
	Neutral,
	Walk,
	Jump,
	Fall,
	Dash,
	Attack1,
	Attack2,
	Attack3,
	Damage,
	Death
};

// キャラクターのパラメータ
struct CharacterParameter
{
	// 体力
	int hp;

	// 移動速度
	float speed;

	// 攻撃力
	int attackPow;

	// 防御力
	int defensePow;

	// ダッシュ力
	float dushPow;

	// ジャンプ力
	float jumpPow;

	CharacterParameter() :
		hp(0), speed(0.0f), attackPow(0), defensePow(0), 
		dushPow(0.0f), jumpPow(0.0f) {}

	CharacterParameter(const int hp, const float speed, const int attackPow,
		const int defencePow, const float dushPow, const float jumpPow) :
		hp(hp), speed(speed), attackPow(attackPow), defensePow(defencePow), 
		dushPow(dushPow), jumpPow(jumpPow) {}

	CharacterParameter(const float speed, const int attackPow,
		const int defencePow, const float dushPow, const float jumpPow) :
		speed(speed), attackPow(attackPow), defensePow(defencePow), 
		dushPow(dushPow), jumpPow(jumpPow) {}
};

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
	void ChangeState(const CharacterState state);

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

	// ステータス取得
	CharacterState GetState()const;

	// パラメータ取得
	CharacterParameter GetParam()const;

	// 反転フラグセット
	void SetTurnFlag(const bool flag);

	// ダメージセット
	void SetDamage(const int attackPow, const int defensePow);
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

	// ライブラリ
	std::weak_ptr<MyLib> lib;

	// カメラ弱参照
	std::weak_ptr<Camera> cam;

	// キャラハンドル
	Texture tex;

	// 状態
	CharacterState state;

	// 1つ前の状態
	CharacterState oldState;

	// 状態コンテナ
	std::unordered_map<CharacterState, std::string> stMap;

	// ワールド座標
	Vec2f worldPos;

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
	std::unordered_map<CharacterState, std::function<void(void)>> func;

	// データ
	std::weak_ptr<std::unordered_map<std::string, Data>> info;

	// アニメーション止める
	bool stopFlag;

	// キャラクターパラメータ
	CharacterParameter cParam;

	// 無敵フラグ
	bool invincibleFlag;

	// ノックバック距離
	float knockBackRange;

	// ダメージ
	int damage;
private:
	// 状態初期化
	void InitState();

	// 矩形
	std::unordered_map<CharacterState, std::vector<std::vector<Primitive>>> box;
};