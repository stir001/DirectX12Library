#include "stdafx.h"
#include "FbxAnimationConverter.h"
#include "FbxAnimationData.h"
#include "FbxStructure.h"
#include "FbxsdkHaveStruct.h"

FbxAnimationConverter::FbxAnimationConverter()
{
}

FbxAnimationConverter::~FbxAnimationConverter()
{
}

FbxAnimationData* FbxAnimationConverter::ConvertAnimationData(const std::vector<Fbx::SkeletonAnimationData>& data, long long int maxframe, const std::string& animationPath)
{
	mAnimation = new FbxAnimationData();
	mAnimation->mMaxFrame = static_cast<int>(maxframe);
	mAnimation->mAnimationDataPath = animationPath;

	mAnimation->mAnimData.resize(data.size());
	for (unsigned int i = 0; i < mAnimation->mAnimData.size();++i)
	{
		mAnimation->mAnimData[i].boneName = data[i].skeletonName;
		mAnimation->mAnimData[i].frameData.resize(data[i].animMatrix.size());
		for (unsigned int j = 0; j <mAnimation->mAnimData[i].frameData.size(); ++j)
		{
			mAnimation->mAnimData[i].frameData[j].matrix = data[i].animMatrix[j].matrix;
			mAnimation->mAnimData[i].frameData[j].frame = data[i].animMatrix[j].frame;
		}
	}

	return mAnimation;
}