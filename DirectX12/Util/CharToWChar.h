#pragma once
#include <string>

size_t ToWChar(std::wstring& wstr, std::string cstr);

size_t ToChar(std::string& cstr, std::wstring wstr);