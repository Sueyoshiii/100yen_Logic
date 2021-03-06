#pragma once
#include <string>
#include <memory>
#include <unordered_map>

class Queue;
class List;
class Fence;

class Compute
{
	// 情報
	struct Info {
		//リソースID
		int rsc;
		//送信データ
		void* data;
	};
public:
	// コンストラクタ
	Compute();
	// デストラクタ
	virtual ~Compute();

protected:
	// シェーダの読み込み
	void Load(const std::string& name, const std::wstring& fileName);

	// CBVの生成
	void CBV(const std::string& name, const size_t& size, const unsigned int& index);

	// UAVの生成
	void UAV(const std::string& name, const size_t& stride, const size_t& num, const unsigned int& index);

	// マップ
	long Map(const std::string& name);

	// アンマップ
	void Unmap(const std::string& name);

	// 終了
	void End(void);


	// キュー
	std::shared_ptr<Queue>queue;

	// リスト
	std::unique_ptr<List>list;

	// フェンス
	std::unique_ptr<Fence>fence;

	// ヒープID
	int heap;

	// リソース情報
	std::unordered_map<std::string, Info>info;
};
