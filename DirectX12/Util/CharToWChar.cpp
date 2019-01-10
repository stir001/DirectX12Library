#include "stdafx.h"
#include "CharToWChar.h"

#include <wpframework.h>
#include <locale.h>
#include <stdlib.h>

size_t ToWChar(std::wstring& wstr, std::string cstr)
{
	size_t rtn = 0;

	cstr.push_back('\0');
	wchar_t* buf = new wchar_t[cstr.size()];
	mbstowcs_s(&rtn, buf, cstr.size(), cstr.data(), _TRUNCATE);
	wstr.resize(rtn);
	wstr = buf;
	delete buf;
	return rtn;
}

size_t ToChar(std::string& cstr, std::wstring wstr)
{	
	size_t rtn = 0;
	wstr.push_back('\0');
	char* buf = new char[wstr.size()];
	wcstombs_s(&rtn, buf, wstr.size(), wstr.data(), _TRUNCATE);
	cstr.resize(rtn);
	cstr = buf;
	delete buf;
	return rtn;
}
