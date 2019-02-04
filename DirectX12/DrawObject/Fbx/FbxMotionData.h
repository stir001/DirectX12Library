#pragma once
#include "FbxStructure.h"

class FbxMotionConverter;
class FbxMotionPlayer;
class FbxConverter;

class FbxMotionData
{
	friend FbxMotionConverter;
	friend FbxMotionPlayer;
	friend FbxConverter;
public:
	FbxMotionData();
	~FbxMotionData();

	std::string GetMotionDataPath() const;
private:
	int mMaxFrame;
	Fbx::AnimationDatas_t mAnimData;
	std::string mMotionDataPath;
};

