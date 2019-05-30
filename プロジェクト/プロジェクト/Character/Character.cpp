#include "Character.h"

const float Const::GR = 0.98f;
const float Const::GROUND = 500.0f;

// �R���X�g���N�^
Character::Character() :
	state(ST::Neutral), oldState(state), localPos(Vec2f()), vel(Vec2f()), alpha(1.0f),
	turnFlag(false), frame(0), animCnt(0), index(0), stopFlag(false),
	speed(1.0f), dushPow(1.0f), jumpPow(1.0f), invincibleFlag(false)
{
	InitState();
	bAlpha = 0.5f;
}

// �f�X�g���N�^
Character::~Character()
{
}

// �X�e�[�W���ɍ��W��␳
void Character::CorrectPosInStage()
{
	float left  = Stage::Get().GetRange().Left();
	float right = Stage::Get().GetRange().Right();

	localPos.x = std::min(std::max(localPos.x, left), right - tex.size.x);
}

// ����
void Character::FallUpdate()
{
	vel.y += Const::GR;
	localPos.y += vel.y;
	localPos.y = std::min(localPos.y, Const::GROUND);
}

// ��ԑJ��
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

// �L�����N�^�[�f�[�^�ǂݍ���
void Character::LoadData(const std::string& filePath)
{
	Info::Get().Load(filePath);
	info = Info::Get().GetData(filePath);
}

// �L�����N�^�[�摜�ǂݍ���
void Character::LoadImage(const std::string& filePath)
{
	tex.Load(filePath);
	tex.size    = info.lock()->at(stMap[state]).rect[index].anim.size;
	tex.divSize = info.lock()->at(stMap[state]).rect[index].anim.size;
}

// �摜�`��
void Character::DrawImage()
{
	tex.offsetPos = {
		tex.offsetPos.x = tex.divSize.x * index,
		info.lock()->at(stMap[state]).rect[index].anim.pos.y
	};
	lib.lock()->Draw(tex, alpha, turnFlag);
}

// �Փˋ�`�`��
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

// �A�j���[�V�����X�V
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

// �A�j���[�V�����̏I���𒲂ׂ�
bool Character::CheckAnimEnd()
{
	if (index >= info.lock()->at(stMap[state]).rect.size() - 1 &&
		frame >= info.lock()->at(stMap[state]).animTime)
	{
		return true;
	}

	return false;
}

// ���[�J�����W�̍X�V
void Character::UpdateLocalPos()
{
	tex.pos = cam.lock()->Correction(localPos);
}

// ���G����
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

// �m�b�N�o�b�N
void Character::KnockBack(const Vec2f& vec)
{
	float v = vec.x != 0.0f ? vec.x / std::fabs(vec.x) : -1.0f;
	vel = Vec2f(speed / 2.0f * v, jumpPow / 2.0f);
}

// �X�e�[�^�X������
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

// �Փˋ�`���擾
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

// ���W�擾
Vec2f Character::GetPos() const
{
	return tex.pos;
}

// �T�C�Y�擾
Vec2f Character::GetSize() const
{
	return tex.size;
}

// ���]�t���O�擾
bool Character::GetTurnFlag() const
{
	return turnFlag;
}

// ���G�t���O�擾
bool Character::GetInvincibleFlag() const
{
	return invincibleFlag;
}

// ���]�t���O�Z�b�g
void Character::SetTurnFlag(const bool flag)
{
	turnFlag = flag;
}

