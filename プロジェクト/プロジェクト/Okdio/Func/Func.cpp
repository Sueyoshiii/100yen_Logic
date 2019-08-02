#include "Func.h"
#include "../XAudio2/XAudio2.h"
#include "../Manager/Manager.h"
#include "../Loader/Loader.h"
#include <windows.h>

// デバッグ出力
void okmonn::DebugStream(const std::string& str)
{
#ifdef _DEBUG
	std::string tmp = str;
	tmp.push_back('\n');
	OutputDebugStringA(tmp.data());
#endif
}

// エンジン開始
long okmonn::EnginStart(void)
{
	return XAudio2::Get().EnginStart();
}

// エンジン終了
void okmonn::EnginEnd(void)
{
	XAudio2::Get().EnginEnd();
}

// オブジェクト生成
long okmonn::CreateObj(const GUID& id, void** obj)
{
	return Manager::Get().CreateObj(id, obj);
}

// 波形データ削除
void okmonn::Delete(const std::string& fileName)
{
	Loader::Get().Delete(fileName);
}

