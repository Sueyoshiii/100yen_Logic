#include "Character.h"

const float Const::GR = 0.98f;
const float Const::GROUND = 500.0f;

// �R���X�g���N�^
Character::Character() :
	state(ST::Neutral), oldState(state), localPos(Vec2f()), vel(Vec2f()),
	turnFlag(false), frame(0), animCnt(0), index(0), stopFlag(false),
	speed(1.0f), dushPow(1.0f), jumpPow(1.0f)
{
	InitState();
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

	lib.lock()->Draw(tex, 1.0f, turnFlag);
}

// �Փˋ�`�`��
void Character::DrawRect()
{
	if (state == ST::Attack3) {
		int a = 0;
	}
	auto s = info.lock()->at(stMap[ST::Attack3]).rect[3].hit.size();
	unsigned int id = 0;
	for (auto& i : info.lock()->at(stMap[state]).rect[index].hit)
	{
		Vec2f pos  = tex.pos + i.rect.pos * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
		Vec2f size = i.rect.size * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;

		box[state][index][id].pos[0] = Vec3f(Vec2f(pos));
		box[state][index][id].pos[1] = Vec3f(Vec2f(pos.x + size.x, pos.y));
		box[state][index][id].pos[2] = Vec3f(Vec2f(pos.x, pos.y + size.y));
		box[state][index][id].pos[3] = Vec3f(Vec2f(pos + size));

		Vec3f color = 0.0f;
		if (i.type == HitType::Damage)
		{
			color.y = 1.0f;
		}
		else
		{
			color.x = 1.0f;
		}
		lib.lock()->Draw(box[state][index][id], color, 0.5f);
		++id;
	}

	//for (unsigned int i = 0; i < info.lock()->at(stMap[state]).rect[index].hit.size(); ++i)
	//{
	//	auto& hit = info.lock()->at(stMap[state]).rect[index].hit[i];
	//	Vec2f pos;
	//	Vec2f size;
	//	if (turnFlag)
	//	{
	//		pos  = Vec2f(tex.pos.x + tex.size.x, tex.pos.y) + Vec2f(-hit.rect.pos.x, hit.rect.pos.y) * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
	//		size = Vec2f(-hit.rect.size.x, hit.rect.size.y) * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
	//		//pos = Vec2f(tex.pos.x - hit.rect.pos.x, tex.pos.y + hit.rect.pos.y);
	//		//size = hit.rect.size;
	//	}
	//	else
	//	{
	//		pos  = tex.pos + hit.rect.pos * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
	//		size = hit.rect.size * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
	//		//pos = tex.pos + hit.rect.pos;
	//		//size = hit.rect.size;
	//	}

	//	float r = 0.0f;
	//	float g = 0.0f;
	//	if (hit.type == HitType::Attack)
	//	{
	//		r = 1.0f;
	//	}
	//	else
	//	{
	//		g = 1.0f;
	//	}

	//	box[state][index][i].pos[0] = Vec3f(Vec2f(pos));
	//	box[state][index][i].pos[1] = Vec3f(Vec2f(pos.x + size.x, pos.y));
	//	box[state][index][i].pos[2] = Vec3f(Vec2f(pos.x, pos.y + size.y));
	//	box[state][index][i].pos[3] = Vec3f(Vec2f(pos + size));

	//	lib.lock()->Draw(box[state][index][i], Vec3f(r, g, 0.0f), 0.5f);
	//}
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
		if (turnFlag)
		{
			rect.rect.pos = Vec2f(tex.pos.x + tex.size.x, tex.pos.y) + Vec2f(-rect.rect.pos.x, rect.rect.pos.y) * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
			rect.rect.size = Vec2f(-rect.rect.size.x, rect.rect.size.y) * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
		}
		else
		{
			rect.rect.pos = tex.pos + rect.rect.pos * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
			rect.rect.size = rect.rect.size * tex.size / info.lock()->at(stMap[state]).rect[index].anim.size;
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

// ���]�t���O�Z�b�g
void Character::SetTurnFlag(const bool flag)
{
	turnFlag = flag;
}

