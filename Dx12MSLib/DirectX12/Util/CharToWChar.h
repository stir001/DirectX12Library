#pragma once
#include <string>

///@fn
///@param ppBuf �ϊ���̕����������o�b�t�@
///@param charWordLen �ԊґO������̒���
///@param pCharWord �ԊґO�̕�����
///@param ppBufMaxCount �ϊ���̕�����o�b�t�@�̍ő啶����
///@return �ϊ�����������̐�
size_t ToWChar(wchar_t** ppBuf, size_t charWordLen, const char* pCharWord, size_t ppBufMaxCount);

size_t ToWChar(wchar_t** ppBuf, std::string cstr);

size_t ToWChar(std::wstring& wstr, std::string cstr);

size_t ToChar(char** ppBuf, size_t ppBufSizeInByte, const wchar_t* wcStr, size_t convertWordNum);