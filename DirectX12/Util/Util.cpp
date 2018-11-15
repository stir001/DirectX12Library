#include "stdafx.h"
#include "Util.h"
#include <DirectXMath.h>

std::string GetRelativePath(const std::string& path)
{
	size_t relative = path.rfind('/') + 1;
	std::string rtn;
	rtn.resize(relative);
	std::copy(path.begin(),path.begin() + relative,rtn.begin());
	return rtn;
}

template <typename T>
std::string ConvertNumToString(T num, const unsigned int maxDigit)
{
	std::string rtn;
	rtn.resize(maxDigit);
	unsigned int extractDigit = 10;

	for (unsigned int i = 0; i < maxDigit; ++i)
	{
		T currentDigit = num % extractDigit;
		rtn[maxDigit - 1 - i] = currentDigit + '0';
		num /= 10;
	}
	return rtn;
}

std::string ConvertNumberToString(unsigned int num)
{
	const unsigned int MAX_DIGIT = 10;
	return ConvertNumToString(num,MAX_DIGIT);
}

std::string ConvertNumberToString(unsigned char num)
{
	const unsigned int MAX_DIGIT = 3;
	return ConvertNumToString(num,MAX_DIGIT);
}
