#pragma once
#include <string>
#include <iostream>

class File
{
private:
	FILE* mFp;
public:
	void SetFile(const std::string& path);
	template <typename T>
	void LoadFile(T* buffer, int readcount = 1,int size = -1);
	void SeekFile(int seeksize);
	void Close();
	File(std::string path);
	~File();
};

template <typename T>
void File::LoadFile(T* buffer, int readcount, int size)
{
	if (size == -1) size = sizeof(*buffer);
	for (int i = 0; i < readcount; ++i)
	{
		fread(&buffer[i], size, 1, mFp);
	}
};