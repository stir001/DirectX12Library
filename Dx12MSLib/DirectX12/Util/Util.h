#pragma once
#include <string>

namespace DirectX
{
	struct XMFLOAT4X4;
	struct XMMATRIX;
}

std::string GetRelativePath(const std::string& path);

std::string ConvertNumberToString(unsigned int num);

std::string ConvertNumberToString(unsigned char num);