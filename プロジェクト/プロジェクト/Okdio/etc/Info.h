#pragma once

namespace okmonn
{
	// サウンド情報
	struct Info
	{
		//サンプリング周波数
		unsigned short sample;
		//量子化ビット数
		unsigned char bit;
		//チャンネル数
		unsigned char channel;

		Info() : sample(0), bit(0), channel(0) {}
		Info(const Info& info) {
			(*this) = info;
		}
		Info(const unsigned short& s, const unsigned char& b, const unsigned char& c) :
			sample(s), bit(b), channel(c) {}
		void operator=(const Info& info) {
			sample  = info.sample;
			bit     = info.bit;
			channel = info.channel;
		}
	};
}
