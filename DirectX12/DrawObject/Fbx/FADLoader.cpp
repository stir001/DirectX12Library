#include "stdafx.h"
#include "FADLoader.h"
#include "FbxMotionData.h"

#include <fstream>
#include <Windows.h>

FADLoader::FADLoader()
{
}


FADLoader::~FADLoader()
{
}

std::shared_ptr<FbxMotionData> FADLoader::LoadFAD(const std::string & filePath)
{
	auto data = std::make_shared<FbxMotionData>();

	std::ifstream fileStream(filePath, std::ios_base::in | std::ios_base::binary);
	if (!fileStream)
	{
		MessageBoxA(nullptr, "ƒtƒ@ƒCƒ‹‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ", "FMDLoader", MB_OK);
		return nullptr;
	}

	char header[4];
	fileStream.read(header, sizeof(header[0]) * 4);
	std::string cmp = header;
	if ("FAD" != cmp)
	{
		return nullptr;
	}

	fileStream.read(reinterpret_cast<char*>(&data->mMaxFrame), sizeof(data->mMaxFrame));
	unsigned int dataNum = 0;
	fileStream.read(reinterpret_cast<char*>(&dataNum), sizeof(dataNum));
	data->mAnimData.resize(dataNum);
	for (auto& anim : data->mAnimData)
	{
		unsigned int boneNameLength = 0;
		fileStream.read(reinterpret_cast<char*>(&boneNameLength), sizeof(boneNameLength));
		anim.boneName.resize(boneNameLength);
		fileStream.read(reinterpret_cast<char*>(&anim.boneName[0]), sizeof(anim.boneName[0]) * boneNameLength);
		unsigned int frameDataNum = 0;
		fileStream.read(reinterpret_cast<char*>(&frameDataNum), sizeof(frameDataNum));
		anim.frameData.resize(frameDataNum);
		fileStream.read(reinterpret_cast<char*>(anim.frameData.data()), sizeof(anim.frameData[0]) * frameDataNum);
	}

	return data;
}
