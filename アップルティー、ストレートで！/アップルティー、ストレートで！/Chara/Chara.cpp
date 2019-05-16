#include "Chara.h"
#include "../InfoLoader/InfoLoader.h"
#include "../Camera/Camera.h"
#include "../Application/Application.h"
#include <algorithm>

// コンストラクタ
Chara::Chara() :
	state("wait"), index(0), speed(1.0f), flam(0.0f), reverse(false), dead(false)
{
	image.clear();
}
// デストラクタ
Chara::~Chara()
{
}

// ステータスのセット
void Chara::SetState(const std::string & state)
{
	if (this->state == state)
	{
		return;
	}

	this->state = state;
	index       = 0;
	flam        = 0.0f;
	if (state == "damage")
	{
		hp -= (hp != 0) ? 1 : 0;
	}
}

// あたり矩形の取得
std::vector<HitRect<Vec2f>> Chara::GetRect(void)
{
	auto tmp = InfoLoader::Get().GetInfo(info)->at(state).rect[index].hit;
	std::for_each(tmp.begin(), tmp.end(), [&](HitRect<Vec2f>& rect)->void {
		if (reverse == false)
		{
			rect.rect.pos  = pos + rect.rect.pos * size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
			rect.rect.size = rect.rect.size * size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
		}
		else
		{
			rect.rect.pos  = Vec2f(pos.x + size.x, pos.y) + Vec2f(-rect.rect.pos.x, rect.rect.pos.y) * size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
			rect.rect.size = Vec2f(-rect.rect.size.x, rect.rect.size.y) * size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
		}
	});
	
	return tmp;
}

// アニメーション・矩形情報の読み込み
void Chara::LoadInfo(const std::string & fileName)
{
	InfoLoader::Get().Load(fileName);
	info = fileName;
}

// 画像の読み込み
void Chara::LoadImg(const std::string & name, const std::string & fileName)
{
	if (image.find(name) != image.end())
	{
		return;
	}

	app.lock()->LoadTex(image[name], fileName);
}

// アニメーター
void Chara::Animator(const float & speed)
{
	flam += speed;
	if (flam >= InfoLoader::Get().GetInfo(info)->at(state).animTime)
	{
		index = (index + 1 >= InfoLoader::Get().GetInfo(info)->at(state).rect.size()) ? 0 : ++index;
		flam = 0.0f;
	}
}

// アニメーション終了フラグ
bool Chara::CheckAnimEnd(void)
{
	if (flam >= InfoLoader::Get().GetInfo(info)->at(state).animTime - 1.0f
		&& index >= InfoLoader::Get().GetInfo(info)->at(state).rect.size() - 1)
	{
		return true;
	}

	return false;
}

// 画像の描画
void Chara::DrawImg(const std::string & name, const float & alpha, const bool & turnY)
{
	if (image.find(name) == image.end())
	{
		return;
	}
	app.lock()->DrawTex(image[name], lpos, size,
		InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.pos, InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size,
		alpha, reverse, turnY);
}

// あたり矩形の描画
void Chara::DrawRect(void)
{
	for (auto& i : InfoLoader::Get().GetInfo(info)->at(state).rect[index].hit)
	{
		Vec2f pos{};
		Vec2f size{};
		if (reverse == false)
		{
			pos  = lpos + i.rect.pos * this->size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
			size = i.rect.size * this->size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
		}
		else
		{
			pos  = Vec2f(lpos.x + this->size.x, lpos.y) + Vec2f(-i.rect.pos.x, i.rect.pos.y) * this->size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
			size = Vec2f(-i.rect.size.x, i.rect.size.y) * this->size / InfoLoader::Get().GetInfo(info)->at(state).rect[index].anim.size;
		}

		Color color{};
		if (i.type == HitType::damage)
		{
			color.g = 1.0f;
		}
		else
		{
			color.r = 1.0f;
		}
		color.a = 0.5f;
		app.lock()->DrawBox(pos, size, color);
	}
}

// ローカル座標の更新
void Chara::UpDataLocal(void)
{
	lpos = pos + cam.lock()->GetPos();
}

// 画像の削除
void Chara::Delete(const std::string & name)
{
	if (image.find(name) != image.end())
	{
		app.lock()->DeleteTex(image[name]);
		image.erase(image.find(name));
	}
}

// 画像の削除
void Chara::Delete(void)
{
	for (auto itr = image.begin(); itr != image.end(); ++itr)
	{
		app.lock()->DeleteTex(itr->second);
	}
}

void Chara::KnockBack(float dis)
{
	// ノックバック有効中であるか
	static bool kb_flg = false;
	static float kb_dis = 0;
	
	if (dis != 0)
	{
		kb_dis = dis;
		kb_flg = true;
	}

	if (kb_flg == true)
	{
		if (reverse == true)
		{
			pos.x -= kb_dis;
		}
		else
		{
			pos.x += kb_dis;
		}

		kb_dis /= 2;
		
		if (kb_dis < 0.1f)
		{
			kb_flg = false;
			kb_dis = 0;
		}
	}
	
}

