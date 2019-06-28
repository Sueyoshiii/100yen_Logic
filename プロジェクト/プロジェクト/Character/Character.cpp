#include "Character.h"

// コンストラクタ
Character::Character() :
	state("Neutral"), oldState(state), worldPos(Vec2f()), vel(Vec2f()), alpha(1.0f),
	turnFlag(false), frame(0), animCnt(0), index(0), stopFlag(false), damage(0),
	invincibleFlag(false), cParam(CharacterParameter()), knockBackRange(0.0f)
{
	InitState();
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

	worldPos.x = std::min(std::max(worldPos.x, left), right - tex.size.x);
}

// 落下
void Character::FallUpdate()
{
	vel.y += Stage::Get().GetGravity();
	worldPos.y += vel.y;
	worldPos.y = std::min(worldPos.y, Stage::Get().GetGround());
}

// 状態遷移
void Character::ChangeState(const std::string& state)
{
	if (func.find(state) == func.end())
	{
		return;
	}
	frame   = 0;
	animCnt = 0;
	index   = 0;
	this->state = state;

	if (this->state == "Damage")
	{
		cParam.hp -= damage;
	}

	if (box.find(this->state) == box.end())
	{
		box[this->state].resize(info.lock()->at(this->state).rect.size());
		for (unsigned int i = 0; i < box[this->state].size(); ++i)
		{
			box[this->state][i].resize(info.lock()->at(this->state).rect[i].hit.size());
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
	tex.size    = info.lock()->at(state).rect[index].anim.size;
	tex.divSize = info.lock()->at(state).rect[index].anim.size;
}

// 画像描画
void Character::DrawImage()
{
	tex.offsetPos = {
		tex.divSize.x * index,
		info.lock()->at(state).rect[index].anim.pos.y
	};
	lib.lock()->Draw(tex, alpha, turnFlag);
}

// 衝突矩形描画
void Character::DrawRect()
{
	unsigned int id = 0;
	for (auto& i : info.lock()->at(state).rect[index].hit)
	{
		Vec2f pos;
		Vec2f size;
		Vec2f animSize = info.lock()->at(state).rect[index].anim.size;
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

		lib.lock()->Draw(box[state][index][id], color, 0.5f);

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
	if (frame > info.lock()->at(state).animTime)
	{
		index = (index + 1 >= info.lock()->at(state).rect.size()) ? 0 : ++index;
		frame = 0;
	}
}

// アニメーションの終了を調べる
bool Character::CheckAnimEnd(const unsigned int num)
{
	if (index >= info.lock()->at(state).rect.size() - num &&
		frame >= info.lock()->at(state).animTime - (num -1))
	{
		return true;
	}

	return false;
}

// ローカル座標の更新
void Character::UpdateLocalPos()
{
	tex.pos = cam.lock()->Correction(worldPos);
}

// 無敵処理
void Character::InvicibleUpdate()
{
	static unsigned int invincibleCnt = 0;
	if (invincibleFlag)
	{
		if (invincibleCnt > 120)
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
	vel = Vec2f(knockBackRange / 2.0f * v, cParam.jumpPow / 2.0f);
}

// ステータス初期化
void Character::InitState()
{
	//if (stMap.empty())
	//{
	//	stMap.clear();
	//}

	//stMap[CharacterState::Neutral] = "Neutral";
	//stMap[CharacterState::Walk]    = "Walk";
	//stMap[CharacterState::Jump]    = "Jump";
	//stMap[CharacterState::Fall]    = "Fall";
	//stMap[CharacterState::Dash]    = "Dash";
	//stMap[CharacterState::Attack1] = "Attack1";
	//stMap[CharacterState::Attack2] = "Attack2";
	//stMap[CharacterState::Attack3] = "Attack3";
	//stMap[CharacterState::Damage]  = "Damage";
	//stMap[CharacterState::Death]   = "Death";
}

// 衝突矩形を取得
std::vector<HitRect<Vec2f>> Character::GetRect()
{
	auto hit = info.lock()->at(state).rect[index].hit;
	std::for_each(hit.begin(), hit.end(), [&](HitRect<Vec2f> & rect)->void
	{
		Vec2f animSize = info.lock()->at(state).rect[index].anim.size;
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

// ステータス取得
std::string Character::GetState() const
{
	return state;
}

// パラメータ取得
CharacterParameter Character::GetParam() const
{
	return cParam;
}

// 反転フラグセット
void Character::SetTurnFlag(const bool flag)
{
	turnFlag = flag;
}

// ダメージセット
void Character::SetDamage(const int attackPow, const int defensePow)
{
	int value = attackPow - defensePow;

	// 0以下だった場合は1にする
	damage = value > 0 ? value : 1;
}

