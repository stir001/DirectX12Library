#pragma once
#include <string>

///@fn
///@param ppBuf 変換後の文字列を入れるバッファ
///@param charWordLen 返還前文字列の長さ
///@param pCharWord 返還前の文字列
///@param ppBufMaxCount 変換後の文字列バッファの最大文字数
///@return 変換した文字列の数
size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount);

size_t ToWChar(wchar_t** ppBuf, std::string cstr);

size_t ToWChar(std::wstring& wstr, std::string cstr);

size_t ToChar(char** ppBuf, size_t ppBufSizeInByte, const wchar_t* wcStr, size_t convertWordNum);