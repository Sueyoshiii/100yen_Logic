#include "Character.h"

// �R���X�g���N�^
Character::Character() :
	state("Neutral"), oldState(state), worldPos(Vec2f()), vel(Vec2f()), alpha(1.0f),
	turnFlag(false), frame(0), animCnt(0), index(0), stopFlag(false), damage(0),
	invincibleFlag(false), cParam(CharacterParameter()), knockBackRange(0.0f),
	jumpFlag(false)
{
}

// �f�X�g���N�^
Character::~Character()
{
	tex.clear();
	info.clear();
}

// �X�e�[�W���ɍ��W��␳
void Character::CorrectPosInStage()
{
	float left  = StageManager::Get().GetRange().Left();
	float right = StageManager::Get().GetRange().Right();

	worldPos.x = std::max(worldPos.x, left);
	//worldPos.x = std::min(std::max(worldPos.x, left), right - tex.size.x);
}

// ����
void Character::FallUpdate()
{
	vel.y += StageManager::Get().GetGravity();
	worldPos.y += vel.y;
	worldPos.y = std::min(GetFootPos().y, StageManager::Get().GetGround() - tex[type].size.y);
}

// ��ԑJ��
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

	if (box.find(this->state) == box.end() || type != oldType)
	{
		box.clear();
		box[this->state].resize(info[type].lock()->at(this->state).rect.size());
		for (unsigned int i = 0; i < box[this->state].size(); ++i)
		{
			box[this->state][i].resize(info[type].lock()->at(this->state).rect[i].hit.size());
			std::fill(box[this->state][i].begin(), box[this->state][i].end(), Primitive(PrimitiveType::box));
		}
	}
}

// �L�����N�^�[�f�[�^�ǂݍ���
void Character::LoadData(const std::string& filePath)
{
	Info::Get().Load(filePath);
	info[type] = Info::Get().GetData(filePath);
}

// �L�����N�^�[�摜�ǂݍ���
void Character::LoadImage(const std::string& filePath)
{
	tex[type].Load(filePath);
	tex[type].size    = info[type].lock()->at(state).rect[index].anim.size;
	tex[type].divSize = info[type].lock()->at(state).rect[index].anim.size;
}

// �摜�`��
void Character::DrawImage()
{
	tex[type].offsetPos = {
		tex[type].divSize.x * index,
		info[type].lock()->at(state).rect[index].anim.pos.y
	};

	lib.lock()->Draw(tex[type], alpha, turnFlag);
}

// �Փˋ�`�`��
void Character::DrawRect()
{
	unsigned int id = 0;
	for (auto& i : info[type].lock()->at(state).rect[index].hit)
	{
		Vec2f pos;
		Vec2f size;
		Vec2f animSize = info[type].lock()->at(state).rect[index].anim.size;
		if (turnFlag)
		{
			pos  = Vec2f(tex[type].pos.x + tex[type].size.x, tex[type].pos.y) + Vec2f(-i.rect.pos.x, i.rect.pos.y) * tex[type].size / animSize;
			size = Vec2f(-i.rect.size.x, i.rect.size.y) * tex[type].size / animSize;
		}
		else
		{
			pos  = tex[type].pos + i.rect.pos * tex[type].size / animSize;
			size = i.rect.size * tex[type].size / animSize;
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

// �A�j���[�V�����X�V
void Character::AnimationUpdate()
{
	if (stopFlag)
	{
		return;
	}
	++frame;
	if (frame > info[type].lock()->at(state).animTime)
	{
		index = (index + 1 >= info[type].lock()->at(state).rect.size()) ? 0 : ++index;
		frame = 0;
	}
}

// �A�j���[�V�����̏I���𒲂ׂ�
bool Character::CheckAnimEnd(const unsigned int num)
{
	if (index >= info[type].lock()->at(state).rect.size() - num &&
		frame >= info[type].lock()->at(state).animTime - (num -1))
	{
		return true;
	}

	return false;
}

// ���[�J�����W�̍X�V
void Character::UpdateLocalPos()
{
	tex[type].pos = cam.lock()->Correction(worldPos);
}

// ���G����
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

// �m�b�N�o�b�N
void Character::KnockBack(const Vec2f& vec)
{
	float v = vec.x != 0.0f ? vec.x / std::fabs(vec.x) : -1.0f;
	vel = Vec2f(knockBackRange / 2.0f * v, -25.0f);
}

// �������擾
bool Character::CheckAlive()
{
	if (state != "Death")
	{
		return true;
	}

	return false;
}

// �Փˋ�`���擾
std::vector<HitRect<Vec2f>> Character::GetRect()
{
	auto hit = info[type].lock()->at(state).rect[index].hit;
	std::for_each(hit.begin(), hit.end(), [&](HitRect<Vec2f> & rect)->void
	{
		Vec2f animSize = info[type].lock()->at(state).rect[index].anim.size;
		if (turnFlag)
		{
			rect.rect.pos = Vec2f(tex[type].pos.x + tex[type].size.x, tex[type].pos.y) + Vec2f(-rect.rect.pos.x, rect.rect.pos.y) * tex[type].size / animSize;
			rect.rect.size = Vec2f(-rect.rect.size.x, rect.rect.size.y) * tex[type].size / animSize;
		}
		else
		{
			rect.rect.pos = tex[type].pos + rect.rect.pos * tex[type].size / animSize;
			rect.rect.size = rect.rect.size * tex[type].size / animSize;
		}
	});

	return hit;
}

// ���W�擾
Vec2f Character::GetPos()
{
	return tex[type].pos;
}

// �T�C�Y�擾
Vec2f Character::GetSize()
{
	return tex[type].size;
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

// �X�e�[�^�X�擾
std::string Character::GetState() const
{
	return state;
}

// �p�����[�^�擾
CharacterParameter Character::GetParam() const
{
	return cParam;
}

// �����擾
Vec2f Character::GetFootPos()
{
	return Vec2f(worldPos.x + tex[type].size.x / 2, worldPos.y + tex[type].size.y);
}

// ���]�t���O�Z�b�g
void Character::SetTurnFlag(const bool flag)
{
	turnFlag = flag;
}

// �_���[�W�Z�b�g
void Character::SetDamage(const int attackPow, const int defensePow)
{
	int value = attackPow - defensePow;

	// 0�ȉ��������ꍇ��1�ɂ���
	damage = value > 0 ? value : 1;
}

