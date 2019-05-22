#include "Info.h"

Info::Info()
{
}

Info::~Info()
{
}

Info& Info::Get()
{
	static Info instance;
	return instance;
}

// �ǂݍ���
int Info::Load(const std::string& filePath)
{
	if (data.find(filePath) != data.end())
	{
		return 0;
	}

	FILE* file = nullptr;
	if (fopen_s(&file, filePath.c_str(), "rb") != 0)
	{
		return -1;
	}

	data[filePath] = std::make_shared<std::unordered_map<std::string, Data>>();

	// �X�e�[�^�X���擾
	unsigned int stateNum = 0;
	fread(&stateNum, sizeof(stateNum), 1, file);

	for (unsigned int i = 0; i < stateNum; ++i)
	{
		// �X�e�[�^�X���̕������擾
		unsigned char nameNum = 0;
		fread(&nameNum, sizeof(nameNum), 1, file);

		// �X�e�[�^�X���擾
		std::string state;
		state.resize(nameNum);
		fread(&state[0], state.size(), 1, file);
		data[filePath]->insert_or_assign(state, Data());
		data[filePath]->at(state).state = state;

		// �A�j���[�V�����ڍs�t���[���擾
		float frame = 0.0f;
		fread(&frame, sizeof(frame), 1, file);
		data[filePath]->at(state).animTime = frame;

		// �A�j���[�V�������擾
		unsigned int animNum = 0;
		fread(&animNum, sizeof(animNum), 1, file);

		// �A�j���[�V������`�擾
		Rect<Vec2> anim{};
		data[filePath]->at(state).rect.resize(animNum);
		for (auto& a : data[filePath]->at(state).rect)
		{
			fread(&anim, sizeof(anim), 1, file);
			a.anim.pos  = Vec2f(anim.pos.x, anim.pos.y);
			a.anim.size = Vec2f(anim.size.x, anim.size.y);

			// �Փˋ�`���擾
			unsigned int rectNum = 0;
			fread(&rectNum, sizeof(rectNum), 1, file);
			a.hit.resize(rectNum);

			// �Փˋ�`�擾
			HitRect<Vec2> hit{};
			for (auto& r : a.hit)
			{
				fread(&hit, sizeof(hit), 1, file);
				r.type      = hit.type;
				r.rect.pos  = Vec2f(hit.rect.pos.x, hit.rect.pos.y);
				r.rect.size = Vec2f(hit.rect.size.x, hit.rect.size.y);
			}
		}

		fclose(file);
	}

	return 0;
}

std::shared_ptr<std::unordered_map<std::string, Data>> Info::GetData(const std::string& filePath)
{
	return data[filePath];
}
