#include "InfoLoader.h"

// コンストラクタ
InfoLoader::InfoLoader()
{
	info.clear();
}

// デストラクタ
InfoLoader::~InfoLoader()
{
}

// 読み込み
int InfoLoader::Load(const std::string & fileName)
{
	if (info.find(fileName) != info.end())
	{
		return 0;
	}

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	info[fileName] = std::make_shared<std::unordered_map<std::string, Info>>();

	//ステータス数の読み込み
	unsigned char num = 0;
	fread(&num, sizeof(num), 1, file);

	for (unsigned char i = 0; i < num; ++i)
	{
		//ステータス名の文字数の読み込み
		unsigned char stNameNum = 0;
		fread(&stNameNum, sizeof(stNameNum), 1, file);

		//ステータス名の読み込み
		std::string st;
		st.resize(stNameNum);
		fread(&st[0], st.size(), 1, file);
		info[fileName]->insert_or_assign(st, Info());
		info[fileName]->at(st).st = st;

		//アニメーション移行フレームの読み込み
		float time = 0.0f;
		fread(&time, sizeof(time), 1, file);
		info[fileName]->at(st).animTime = time;

		//アニメーション数の読み込み
		unsigned char animNum = 0;
		fread(&animNum, sizeof(animNum), 1, file);

		//アニメーション矩形の読み込み
		Rect<Vec2>anim{};
		info[fileName]->at(st).rect.resize(animNum);
		for (auto& n : info[fileName]->at(st).rect)
		{
			fread(&anim, sizeof(anim), 1, file);
			n.anim.pos  = { static_cast<float>(anim.pos.x), static_cast<float>(anim.pos.y) };
			n.anim.size = { static_cast<float>(anim.size.x), static_cast<float>(anim.size.y) };

			//あたり矩形数の読み込み
			unsigned char hitNum = 0;
			fread(&hitNum, sizeof(hitNum), 1, file);
			n.hit.resize(hitNum);

			//あたり矩形の読み込み
			HitRect<Vec2>hit{};
			for (auto& m : n.hit)
			{
				fread(&hit, sizeof(hit), 1, file);
				m.type = hit.type;
				m.rect.pos  = { static_cast<float>(hit.rect.pos.x), static_cast<float>(hit.rect.pos.y) };
				m.rect.size = { static_cast<float>(hit.rect.size.x), static_cast<float>(hit.rect.size.y) };
			}
		}
	}

	fclose(file);

	return 0;
}

// 書き込み
int InfoLoader::Write(const std::string & fileName, const std::vector<Info>& info)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "wb") != 0)
	{
		return -1;
	}

	//ステータス数の書き込み
	unsigned char num = static_cast<unsigned char>(info.size());
	fwrite(&num, sizeof(num), 1, file);

	for (unsigned char i = 0; i < num; ++i)
	{
		//ステータス名の文字数の書き込み
		unsigned char stNameNum = static_cast<unsigned char>(info[i].st.size());
		fwrite(&stNameNum, sizeof(stNameNum), 1, file);

		//ステータス名の書き込み
		std::string st = info[i].st;
		fwrite(&st[0], st.size(), 1, file);

		//アニメーション移行フレームの書き込み
		float time = info[i].animTime;
		fwrite(&time, sizeof(time), 1, file);

		//アニメーション数の書き込み
		unsigned char animNum = static_cast<unsigned char>(info[i].rect.size());
		fwrite(&animNum, sizeof(animNum), 1, file);

		//アニメーション矩形の読み込み
		Rect<Vec2>anim{};
		for (auto& n : info[i].rect)
		{
			anim.pos  = { static_cast<int>(n.anim.pos.x), static_cast<int>(n.anim.pos.y) };
			anim.size = { static_cast<int>(n.anim.size.x), static_cast<int>(n.anim.size.y) };
			fwrite(&anim, sizeof(anim), 1, file);

			//あたり矩形数の書き込み
			unsigned char hitNum = static_cast<unsigned char>(n.hit.size());
			fwrite(&hitNum, sizeof(hitNum), 1, file);

			//あたり矩形の読み込み
			HitRect<Vec2>hit{};
			for (auto& m : n.hit)
			{
				hit.type = m.type;
				hit.rect.pos  = { static_cast<int>(m.rect.pos.x), static_cast<int>(m.rect.pos.y) };
				hit.rect.size = { static_cast<int>(m.rect.size.x), static_cast<int>(m.rect.size.y) };
				fwrite(&hit, sizeof(hit), 1, file);
			}
		}
	}

	fclose(file);

	return 0;
}
