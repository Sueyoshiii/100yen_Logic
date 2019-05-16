#pragma once
#include "../Typedef.h"
#include <memory>
#include <functional>
#include <unordered_map>

class Camera;
class Application;

class Chara
{
public:
	// コンストラクタ
	Chara();
	// デストラクタ
	virtual ~Chara();

	// 描画
	virtual void Draw(void) = 0;

	// 処理
	virtual void UpData(void) = 0;

	// 座標の取得
	Vec2f GetPos(void) const {
		return pos;
	}
	// 座標のセット
	void SetPos(const Vec2f& pos) {
		this->pos = pos;
	}
	// サイズの取得
	Vec2f GetSize(void) const {
		return size;
	}
	// 中心座標の取得
	Vec2f GetCenter(void) const {
		return pos + size / 2.0f;
	}
	// ステータスの取得
	std::string GetState(void) const {
		return state;
	}
	// ステータスのセット
	void SetState(const std::string& state);
	// あたり矩形の取得
	std::vector<HitRect<Vec2f>> GetRect(void);
	// 体力の取得
	unsigned int GetHp(void) const {
		return hp;
	}
	// 体力のセット
	void SetHp(const unsigned int& hp) {
		this->hp = hp;
	}
	// 反転フラグの取得
	bool GetReverse(void) const {
		return reverse;
	}
	// 反転フラグのセット
	void SetReverse(const bool& flag) {
		reverse = flag;
	}
	// 死亡フラグの取得
	bool GetDead(void) const {
		return dead;
	}

protected:
	// アニメーション・矩形情報の読み込み
	void LoadInfo(const std::string& fileName);

	// 画像の読み込み
	void LoadImg(const std::string& name, const std::string& fileName);

	// アニメーター
	void Animator(const float& speed = 1.0f);

	// アニメーション終了フラグ
	bool CheckAnimEnd(void);

	// 画像の描画
	void DrawImg(const std::string& name, const float& alpha = 1.0f, const bool& turnY = false);

	// あたり矩形の描画
	void DrawRect(void);

	// ローカル座標の更新
	void UpDataLocal(void);

	// 画像の削除
	void Delete(const std::string& name);
	void Delete(void);

	// 値に応じた距離のヒットバック	
	void KnockBack(float dis);

	bool KnockBackEnd;

	// アプリケーション
	std::weak_ptr<Application>app;

	// カメラ
	std::weak_ptr<Camera>cam;

	// 座標
	Vec2f pos;

	// ローカル座標
	Vec2f lpos;

	// サイズ
	Vec2f size;

	// ステータス
	std::string state;

	// infoファイル名
	std::string info;

	// 体力
	unsigned int hp;

	// 配列用インデックス
	unsigned int index;

	// 移動速度
	float speed;

	// フレーム
	float flam;

	// 反転フラグ
	bool reverse;

	// 死亡フラグ
	bool dead;

	// 画像ID
	std::unordered_map<std::string, int>image;
};
