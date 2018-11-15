#pragma once
#include <iostream>
#include "ImageLoader.h"

class ImageData;

class BmpLoader : public ImageLoader
{
public:
	ImageData LoadImageData(std::string path);
	BmpLoader();
	~BmpLoader();
};

