#pragma once
#include "FbxStructure.h"

class FbxAnimationConverter;
class FbxAnimationPlayer;
class FbxConverter;
class FADLoader;

class FbxAnimationData
{
	friend FbxAnimationConverter;
	friend FbxAnimationPlayer;
	friend FbxConverter;
	friend FADLoader;
public:
	FbxAnimationData();
	~FbxAnimationData();

	std::string GetAnimationDataPath() const;
private:
	int mMaxFrame;
	Fbx::AnimationDatas_t mAnimData;
	std::string mAnimationDataPath;
};

