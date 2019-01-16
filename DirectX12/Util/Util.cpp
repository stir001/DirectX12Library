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
		num /= extractDigit;
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

unsigned int ConvertStringToUINT(std::string num)
{
	const unsigned int digit = static_cast<unsigned int>(num.size());

	unsigned int rtn = 0;

	unsigned int extractDigit = 10;
	for (unsigned int i = 0; i < digit; ++i)
	{
		rtn += (num[i - digit - 1] - '0') * static_cast<unsigned int>(powf(static_cast<float>(extractDigit), static_cast<float>(i)));
	}

	return rtn;
}
