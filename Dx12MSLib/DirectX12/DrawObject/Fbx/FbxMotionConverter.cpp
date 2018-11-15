#include "stdafx.h"
#include "FbxMotionConverter.h"
#include "FbxMotionData.h"
#include "FbxStructure.h"
#include "FbxsdkHaveStruct.h"

FbxMotionConverter::FbxMotionConverter()
{
}

FbxMotionConverter::~FbxMotionConverter()
{
}

FbxMotionData* FbxMotionConverter::ConvertMotionData(const std::vector<Fbx::SkeletonAnimationData>& data, long long int maxframe, const std::string& animationPath)
{
	mMotion = new FbxMotionData();
	mMotion->mMaxFrame = static_cast<int>(maxframe);
	mMotion->mMotionDataPath = animationPath;

	mMotion->mAnimData.resize(data.size());
	for (unsigned int i = 0; i < mMotion->mAnimData.size();++i)
	{
		mMotion->mAnimData[i].boneName = data[i].skeletonName;
		mMotion->mAnimData[i].frameData.resize(data[i].animMatrix.size());
		for (unsigned int j = 0; j <mMotion->mAnimData[i].frameData.size(); ++j)
		{
			mMotion->mAnimData[i].frameData[j].matrix = data[i].animMatrix[j].matrix;
			mMotion->mAnimData[i].frameData[j].frame = data[i].animMatrix[j].frame;
		}
	}

	return mMotion;
}