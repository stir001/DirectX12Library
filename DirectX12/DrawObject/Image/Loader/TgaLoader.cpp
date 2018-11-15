#include "TgaLoader.h"
#include "ImageData.h"
#include "Color.h"

TgaLoader::TgaLoader()
{
}


TgaLoader::~TgaLoader()
{
}

//ImageData TgaLoader::LoadImageData(std::string path)
//{
//	ImageData imgdata;
//	FILE* tgafp = nullptr;
//	fopen_s(&tgafp, path.data(), "rb");
//
//	fread(&header, sizeof(header), 1, tgafp);
//
//	std::vector<COLORRGBA> color(header.width * header.height);
//	LoadRGBA(color, tgafp);
//
//	fclose(tgafp);
//	return imgdata;
//}

void TgaLoader::LoadRGBA(std::vector<COLORRGBA>& buffer, FILE* fp)
{
	int widthfirst, widthend,widthIncrement ,heightfirst, heidghtend,heightIncrement;
	if ((header.descriptor & 0x10) == 0x10)//1:âEÇ©ÇÁç∂ 0:ç∂Ç©ÇÁâE
	{//1
		widthfirst = header.width - 1;
		widthend = 0;
		widthIncrement = -1;
	}
	else
	{//0
		widthfirst = 0;
		widthend = header.width - 1;
		widthIncrement = 1;
	}

	if ((header.descriptor & 0x20) == 0x20)//1:è„Ç©ÇÁâ∫ 0:â∫Ç©ÇÁè„
	{//1
		heightfirst = 0;
		heidghtend = header.height - 1;
		heightIncrement = 1;
	}
	else
	{//0
		heightfirst = header.height - 1;
		heidghtend = 0;
		heightIncrement = -1;
	}

	int winc;
	int hinc;

	for (int i = widthfirst + heightfirst * header.width; i != widthend + heidghtend * header.width; i = (i + widthIncrement) % header.width + ((i + widthIncrement) % header.width == 0 ? ((i / header.width) + heightIncrement) * header.width : i / header.width * header.width))
	{
		fread(&buffer[i].b, sizeof(unsigned char), 1, fp);
		fread(&buffer[i].g, sizeof(unsigned char), 1, fp);
		fread(&buffer[i].r, sizeof(unsigned char), 1, fp);
		fread(&buffer[i].a, sizeof(unsigned char), 1, fp);
		winc = (i + widthIncrement) % header.width;
		hinc = ((i + widthIncrement) % header.width == 0 ? ((i / header.width) + heightIncrement) * header.width : i / header.width * header.width);
	}
}
