#include "stdafx.h"
#include "VMDLoader.h"
#include "VMDAnimation.h"
#include "Util/File.h"

#include <algorithm>

VMDLoader::VMDLoader():mFp(nullptr)
{
}

VMDLoader::~VMDLoader()
{
	if (mFp != nullptr)
	{
		mFp->Close();
		mFp = nullptr;
	}
	mLoadingAnimation = nullptr;
}

std::shared_ptr<VMDAnimation> VMDLoader::LoadAnimation(const std::string& path)
{
	if (mAnimations.find(path) != mAnimations.end()) return mAnimations[path];
	if (mFp == nullptr)
	{
		mFp = std::make_shared<File>(path);
	}
	else
	{
		mFp->SetLoadFile(path);
	}
	mLoadingAnimation = std::make_shared<VMDAnimation>();

	LoadHeader();
	LoadAnimationDatas();

	mFp->Close();

	CreatePoses();
	mAnimations[path] = mLoadingAnimation;
	return mLoadingAnimation;
}

void VMDLoader::LoadHeader()
{
	mFp->LoadFile(&mLoadingAnimation->mHeader.header[0], sizeof(mLoadingAnimation->mHeader.header));
	mFp->LoadFile(&mLoadingAnimation->mHeader.modelName[0], sizeof(mLoadingAnimation->mHeader.modelName));
	mFp->LoadFile(&mLoadingAnimation->mHeader.count);
}

void VMDLoader::LoadAnimationDatas()
{
	mLoadingAnimation->mAnimations.resize(mLoadingAnimation->mHeader.count);
	for (unsigned int i = 0; i < mLoadingAnimation->mHeader.count; i++)
	{
		mFp->LoadFile(&mLoadingAnimation->mAnimations[i].boneName[0],sizeof(mLoadingAnimation->mAnimations[i].boneName));
		mFp->LoadFile(&mLoadingAnimation->mAnimations[i].frameNo);
		mFp->LoadFile(&mLoadingAnimation->mAnimations[i].location);
		mFp->LoadFile(&mLoadingAnimation->mAnimations[i].quoternion);
		mFp->LoadFile(&mLoadingAnimation->mAnimations[i].interpolation[0],sizeof(mLoadingAnimation->mAnimations[i].interpolation));
	}
}

void VMDLoader::CreatePoses()
{
	for (auto& m : mLoadingAnimation->mAnimations)
	{
		std::string boneName = m.boneName;
		VMDPose p;
		p.frameNo = m.frameNo;
		p.quoternion = m.quoternion;
		p.location = m.location;
		mLoadingAnimation->mPoses[boneName].push_back(p);
	}

	for (auto& p : mLoadingAnimation->mPoses)
	{
		std::sort(p.second.begin(), p.second.end(), [&](const VMDPose& lval, const VMDPose& rval) {return lval.frameNo < rval.frameNo; });
	}
}