#include "Character.h"

Character::Character() :
	state(ST::Neutral), oldState(state), localPos(Vec2f()), vel(Vec2f()),
	turnFlag(false), frame(0), animCnt(0), index(0), stopFlag(false)
{
	box = Primitive(PrimitiveType::box);
	InitState();
}

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
		tex.offsetPos.x = tex.divSize.x * frame,
		info.lock()->at(stMap[state]).rect[index].anim.pos.y
	};

	lib.lock()->Draw(tex, 1.0f, turnFlag);
}

// 衝突矩形描画
void Character::DrawRect()
{
	for (auto& i : info.lock()->at(stMap[state]).rect[index].hit)
	{
		Vec2f pos;
		Vec2f size;
		if (turnFlag)
		{
			//pos  = Vec2f(tex.pos.x + tex.size.x, tex.pos.y) + Vec2f(-i.rect.pos.x, i.rect.pos.y) * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
			//size = Vec2f(-i.rect.size.x, i.rect.size.y) * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
			pos  = Vec2f(tex.pos.x - i.rect.pos.x, tex.pos.y + i.rect.pos.y);
			size = i.rect.size;
		}
		else
		{
			pos = tex.pos + i.rect.pos;
			size = i.rect.size;
		}

		float r = 0.0f;
		float g = 0.0f;
		if (i.type == HitType::Attack)
		{
			r = 1.0f;
		}
		else
		{
			g = 1.0f;
		}

		box.pos[0] = Vec3f(Vec2f(pos));
		box.pos[1] = Vec3f(Vec2f(pos.x + size.x, pos.y));
		box.pos[2] = Vec3f(Vec2f(pos.x, pos.y + size.y));
		box.pos[3] = Vec3f(Vec2f(pos + size));

		lib.lock()->Draw(box, Vec3f(r, g, 0.0f), 1.0f);
	}
}

// アニメーション更新
void Character::AnimationUpdate()
{
	unsigned int animTime = unsigned int(info.lock()->at(stMap[state]).animTime);
	unsigned int animNum  = unsigned int(info.lock()->at(stMap[state]).rect.size());

	if (stopFlag)
	{
		return;
	}

	if (animTime > 0)
	{
		frame = (++animCnt) % animTime == 0 ? (++frame) % animNum : frame;
	}
	if (frame >= animTime)
	{
		index = (index + 1) >= info.lock()->at(stMap[state]).rect.size() ? 0 : ++index;
		frame = 0;
	}
}

// アニメーションの終了を調べる
bool Character::CheckAnimEnd()
{
	if (frame >= info.lock()->at(stMap[state]).rect.size() - 1)
	{
		return true;
	}

	return false;
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
	stMap[ST::Death]   = "Deth";
}

