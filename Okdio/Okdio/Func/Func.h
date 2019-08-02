#pragma once
#include <string>

struct _GUID;
typedef _GUID GUID;
class Okdio;

namespace okmonn
{
	// デバッグ出力
	void DebugStream(const std::string& str);

	// エンジン開始
	long EnginStart(void);

	// エンジン終了
	void EnginEnd(void);

	// オブジェクト生成
	long CreateObj(const GUID& id, void** obj);

	// 波形データ削除
	void Delete(const std::string& fileName);
}
