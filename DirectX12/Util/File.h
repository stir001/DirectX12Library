#pragma once
#include <string>
#include <iostream>

class File
{
public:
	File();
	File(std::string path);
	~File();

	void SetLoadFile(const std::string& path);
	template <typename T>
	void LoadFile(T* buffer, int readcount = 1, int size = -1);

	void SetWriteFile(const std::string& name);
	template <typename T>
	void Write(const T* data, int writeNum = 1, int size = -1);
	void SeekFile(int seeksize);
	void Close();
private:
	FILE* mFp;
};

template <typename T>
void File::LoadFile(T* buffer, int readcount, int size)
{
	if (mFp == nullptr)
	{
		return;
	}
	if (size == -1) size = sizeof(*buffer);
	fread(buffer, size, readcount, mFp);
};

template<typename T>
void File::Write(const T* data, int writeNum, int size)
{
	if (mFp == nullptr)
	{
		return;
	}
	if (size == -1) size = sizeof(*T);
	fwrite(data, size, writeNum, mFp);
}