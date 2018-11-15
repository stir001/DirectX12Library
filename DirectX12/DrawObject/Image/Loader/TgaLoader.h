#pragma once
#include "ImageLoader.h"
#include <vector>

struct COLORRGBA;

struct TgaHeader
{
	unsigned char filedLength;
	unsigned char colorType;//�J���[�}�b�v�̗L����
	unsigned char imageType;//0:�C���[�W���� 1:256�F 2:�t���J���[ 3:���� 9:�C���f�b�N�X�J���[ RLE���k 10:�t���J���[ RLE���k 11: ���� RLE���k
	unsigned short colormapOrigin;

	unsigned char colormapDepth;

	unsigned short xAxisOrigin;
	unsigned short yAxisOrigin;

	unsigned short width;
	unsigned short height;

	unsigned char imageDepht;
	unsigned char descriptor;//0~3���� 4(0:���ォ��E,1:�E�ォ�獶) 5(0:������� 1:�������炵��) 6,7:�C���^���[�Y
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

