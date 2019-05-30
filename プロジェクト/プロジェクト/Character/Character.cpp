#include "Character.h"

const float Const::GR = 0.98f;
const float Const::GROUND = 500.0f;

// コンストラクタ
Character::Character() :
	state(ST::Neutral), oldState(state), localPos(Vec2f()), vel(Vec2f()), alpha(1.0f),
	turnFlag(false), frame(0), animCnt(0), index(0), stopFlag(false),
	speed(1.0f), dushPow(1.0f), jumpPow(1.0f), invincibleFlag(false)
{
	InitState();
	bAlpha = 0.5f;
}

// デストラクタ
Character::~Character()
{
}

// ステージ内に座標を補正
void Character::CorrectPosInStage()
{
	float left  = Stage::Get().GetRange().Left();
	float right = Stage::Get().GetRange().Right();

	localPos.x = std::min(std::max(localPos.x, left), right - tex.size.x);
}

// 落下
void Character::FallUpdate()
{
	vel.y += Const::GR;
	localPos.y += vel.y;
	localPos.y = std::min(localPos.y, Const::GROUND);
}

// 状態遷移
void Character::ChangeState(const ST state)
{
	if (func.find(state) == func.end())
	{
		return;
	}
	frame   = 0;
	animCnt = 0;
	index   = 0;
	this->state = state;

	if (this->state == ST::Damage)
	{
		--hp;
	}

	if (box.find(this->state) == box.end())
	{
		box[this->state].resize(info.lock()->at(stMap[state]).rect.size());
		for (unsigned int i = 0; i < box[this->state].size(); ++i)
		{
			box[this->state][i].resize(info.lock()->at(stMap[state]).rect[i].hit.size());
			std::fill(box[this->state][i].begin(), box[this->state][i].end(), Primitive(PrimitiveType::box));
		}
	}
}

// キャラクターデータ読み込み
void Character::LoadData(const std::string& filePath)
{
	Info::Get().Load(filePath);
	info = Info::Get().GetData(filePath);
}

// キャラクター画像読み込み
void Character::LoadImage(const std::string& filePath)
{
	tex.Load(filePath);
	tex.size    = info.lock()->at(stMap[state]).rect[index].anim.size;
	tex.divSize = info.lock()->at(stMap[state]).rect[index].anim.size;
}

// 画像描画
void Character::DrawImage()
{
	tex.offsetPos = {
		tex.offsetPos.x = tex.divSize.x * index,
		info.lock()->at(stMap[state]).rect[index].anim.pos.y
	};
	lib.lock()->Draw(tex, alpha, turnFlag);
}

// 衝突矩形描画
void Character::DrawRect()
{
	unsigned int id = 0;
	for (auto& i : info.lock()->at(stMap[state]).rect[index].hit)
	{
		Vec2f pos;
		Vec2f size;
		Vec2f animSize = info.lock()->at(stMap[state]).rect[index].anim.size;
		if (turnFlag)
		{
			pos  = Vec2f(tex.pos.x + tex.size.x, tex.pos.y) + Vec2f(-i.rect.pos.x, i.rect.pos.y) * tex.size / animSize;
			size = Vec2f(-i.rect.size.x, i.rect.size.y) * tex.size / animSize;
		}
		else
		{
			pos  = tex.pos + i.rect.pos * tex.size / animSize;
			size = i.rect.size * tex.size / animSize;
		}

		Vec3f color = 0.0f;
		if (i.type == HitType::Damage)
		{
			color.y = 1.0f;
		}
		else
		{
			color.x = 1.0f;
		}

		box[state][index][id].pos[0] = Vec3f(Vec2f(pos));
		box[state][index][id].pos[1] = Vec3f(Vec2f(pos.x + size.x, pos.y));
		box[state][index][id].pos[2] = Vec3f(Vec2f(pos.x, pos.y + size.y));
		box[state][index][id].pos[3] = Vec3f(Vec2f(pos + size));

		lib.lock()->Draw(box[state][index][id], color, bAlpha);

		++id;
	}
}

// アニメーション更新
void Character::AnimationUpdate()
{
	if (stopFlag)
	{
		return;
	}
	++frame;
	if (frame > info.lock()->at(stMap[state]).animTime)
	{
		index = (index + 1 >= info.lock()->at(stMap[state]).rect.size()) ? 0 : ++index;
		frame = 0;
	}
}

// アニメーションの終了を調べる
bool Character::CheckAnimEnd()
{
	if (index >= info.lock()->at(stMap[state]).rect.size() - 1 &&
		frame >= info.lock()->at(stMap[state]).animTime)
	{
		return true;
	}

	return false;
}

// ローカル座標の更新
void Character::UpdateLocalPos()
{
	tex.pos = cam.lock()->Correction(localPos);
}

// 無敵処理
void Character::InvicibleUpdate()
{
	static unsigned int invincibleCnt = 0;
	if (invincibleFlag)
	{
		if (invincibleCnt > 60)
		{
			alpha = 1.0f;
			invincibleCnt = 0;
			invincibleFlag = false;
		}
		else
		{
			++invincibleCnt;
			alpha = (alpha > 0.0f) ? alpha -= 0.1f : 1.0f;
		}
	}
}

// ノックバック
void Character::KnockBack(const Vec2f& vec)
{
	float v = vec.x != 0.0f ? vec.x / std::fabs(vec.x) : -1.0f;
	vel = Vec2f(speed / 2.0f * v, jumpPow / 2.0f);
}

// ステータス初期化
void Character::InitState()
{
	if (stMap.empty())
	{
		stMap.clear();
	}

	stMap[ST::Neutral] = "Neutral";
	stMap[ST::Walk]    = "Walk";
	stMap[ST::Jump]    = "Jump";
	stMap[ST::Dash]    = "Dash";
	stMap[ST::Attack1] = "Attack1";
	stMap[ST::Attack2] = "Attack2";
	stMap[ST::Attack3] = "Attack3";
	stMap[ST::Damage]  = "Damage";
	stMap[ST::Death]   = "Death";
}

// 衝突矩形を取得
std::vector<HitRect<Vec2f>> Character::GetRect()
{
	auto hit = info.lock()->at(stMap[state]).rect[index].hit;
	std::for_each(hit.begin(), hit.end(), [&](HitRect<Vec2f> & rect)->void
	{
		Vec2f animSize = info.lock()->at(stMap[state]).rect[index].anim.size;
		if (turnFlag)
		{
			rect.rect.pos = Vec2f(tex.pos.x + tex.size.x, tex.pos.y) + Vec2f(-rect.rect.pos.x, rect.rect.pos.y) * tex.size / animSize;
			rect.rect.size = Vec2f(-rect.rect.size.x, rect.rect.size.y) * tex.size / animSize;
		}
		else
		{
			rect.rect.pos = tex.pos + rect.rect.pos * tex.size / animSize;
			rect.rect.size = rect.rect.size * tex.size / animSize;
		}
	});

	return hit;
}

// 座標取得
Vec2f Character::GetPos() const
{
	return tex.pos;
}

// サイズ取得
Vec2f Character::GetSize() const
{
	return tex.size;
}

// 反転フラグ取得
bool Character::GetTurnFlag() const
{
	return turnFlag;
}

// 無敵フラグ取得
bool Character::GetInvincibleFlag() const
{
	return invincibleFlag;
}

// 反転フラグセット
void Character::SetTurnFlag(const bool flag)
{
	turnFlag = flag;
}

