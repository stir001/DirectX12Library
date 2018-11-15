#pragma once
#include "ImageLoader.h"
#include "Color.h"
#include <vector>

struct PngHeader {
	__int64 header;
};

enum ChaunkType{
	CT_IHDR = 0x49484452,	//ASCIIでIHDR
	CT_PLTE = 0x504c5445,	//ASCIIでPLTE
	CT_IDAT = 0x49444154,	//ASCIIでIDAT
	CT_IEND = 0x49454e44	//ASCIIでIEND
};

enum FilterType
{
	none,
	sub,
	up,
	average,
	paeth,
};

struct IHDR
{
	unsigned int width;//画像の幅
	unsigned int height;//画像の高さ
	unsigned char depth;//ビット深度1,2,4,8,16
	unsigned char colortype;//1:パレット使用 2:カラー 4:aチャンネル 0,2,3,4,6のみ有効
	unsigned char compressFormat;//圧縮形式
	unsigned char filterFormat;//フィルター手法 0:none 1:sub
	unsigned char interlaceFormat;//インターレース手法
	unsigned int crc;//破損チェック
};

struct PLTE
{
	std::vector<COLORRGB> palettes;
};

struct IDAT
{
	std::vector<char> datas;
};

struct IEND
{
	unsigned int crc;
};

struct PngChunk
{
	unsigned int length;
	unsigned int chunktype;
};

class PngLoader :
	public ImageLoader
{
public:
	PngLoader();
	~PngLoader();
};

