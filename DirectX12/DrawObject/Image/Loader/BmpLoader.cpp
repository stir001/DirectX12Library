#include "BmpLoader.h"
#include "TextureObj.h"
#include "ImageData.h"
#include "Dx12Ctrl.h"
#include <d3d12.h>
#include <vector>

BmpLoader::BmpLoader()
{
}


BmpLoader::~BmpLoader()
{
}

ImageData BmpLoader::LoadImageData(std::string path)
{
	BITMAPFILEHEADER bmpfileheader;
	BITMAPINFOHEADER bmpinfoheader;

	ImageData imgData;

	FILE* fp = nullptr;
	fopen_s(&fp, path.data(), "rb");
	fread(&bmpfileheader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpinfoheader, sizeof(BITMAPINFOHEADER), 1, fp);

	imgData.rgbData.resize(bmpinfoheader.biHeight * bmpinfoheader.biWidth * 4);

	for (int line = bmpinfoheader.biHeight - 1; line >= 0; line--) {
		for (int count = 0; count < bmpinfoheader.biWidth * 4; count += 4) {
			unsigned int address = line*bmpinfoheader.biWidth * 4;
			fread(&imgData.rgbData[address + count + 1], 3, sizeof(unsigned char), fp);
		}
	}
	fclose(fp);

	imgData.width = bmpinfoheader.biWidth;
	imgData.height = bmpinfoheader.biHeight;

	return imgData;
}