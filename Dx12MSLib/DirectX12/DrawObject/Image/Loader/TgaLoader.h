#pragma once
#include "ImageLoader.h"
#include <vector>

struct COLORRGBA;

struct TgaHeader
{
	unsigned char filedLength;
	unsigned char colorType;//カラーマップの有無し
	unsigned char imageType;//0:イメージ無し 1:256色 2:フルカラー 3:白黒 9:インデックスカラー RLE圧縮 10:フルカラー RLE圧縮 11: 白黒 RLE圧縮
	unsigned short colormapOrigin;

	unsigned char colormapDepth;

	unsigned short xAxisOrigin;
	unsigned short yAxisOrigin;

	unsigned short width;
	unsigned short height;

	unsigned char imageDepht;
	unsigned char descriptor;//0~3属性 4(0:左上から右,1:右上から左) 5(0:下から上 1:うえからした) 6,7:インタリーズ
};

class TgaLoader :
	public ImageLoader
{
private:
	TgaHeader header;
public:
	//ImageData LoadImageData(std::string path);
	void LoadRGBA(std::vector<COLORRGBA>& buffer, FILE* fp);
	TgaLoader();
	~TgaLoader();
};

