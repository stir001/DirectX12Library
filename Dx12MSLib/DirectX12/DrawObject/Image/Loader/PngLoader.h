#pragma once
#include "ImageLoader.h"
#include "Color.h"
#include <vector>

struct PngHeader {
	__int64 header;
};

enum ChaunkType{
	CT_IHDR = 0x49484452,	//ASCII��IHDR
	CT_PLTE = 0x504c5445,	//ASCII��PLTE
	CT_IDAT = 0x49444154,	//ASCII��IDAT
	CT_IEND = 0x49454e44	//ASCII��IEND
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
	unsigned int width;//�摜�̕�
	unsigned int height;//�摜�̍���
	unsigned char depth;//�r�b�g�[�x1,2,4,8,16
	unsigned char colortype;//1:�p���b�g�g�p 2:�J���[ 4:a�`�����l�� 0,2,3,4,6�̂ݗL��
	unsigned char compressFormat;//���k�`��
	unsigned char filterFormat;//�t�B���^�[��@ 0:none 1:sub
	unsigned char interlaceFormat;//�C���^�[���[�X��@
	unsigned int crc;//�j���`�F�b�N
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

