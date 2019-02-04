#pragma once
#include "FbxStructure.h"

class FbxMotionConverter;
class FbxMotionPlayer;
class FbxConverter;
class FADLoader;

class FbxMotionData
{
	friend FbxMotionConverter;
	friend FbxMotionPlayer;
	friend FbxConverter;
	friend FADLoader;
public:
	FbxMotionData();
	~FbxMotionData();

	std::string GetMotionDataPath() const;
private:
	int mMaxFrame;
	Fbx::AnimationDatas_t mAnimData;
	std::string mMotionDataPath;
};

