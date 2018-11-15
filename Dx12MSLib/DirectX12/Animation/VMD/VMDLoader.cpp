#include "stdafx.h"
#include "VMDLoader.h"
#include "VMDMotion.h"
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
	mLoadingMotion = nullptr;
}

std::shared_ptr<VMDMotion> VMDLoader::LoadMotion(const std::string& path)
{
	if (mMotions.find(path) != mMotions.end()) return mMotions[path];
	if (mFp == nullptr)
	{
		mFp = std::make_shared<File>(path);
	}
	else
	{
		mFp->SetFile(path);
	}
	mLoadingMotion = std::make_shared<VMDMotion>();

	LoadHeader();
	LoadMotionDatas();

	mFp->Close();

	CreatePoses();
	mMotions[path] = mLoadingMotion;
	return mLoadingMotion;
}

void VMDLoader::LoadHeader()
{
	mFp->LoadFile(&mLoadingMotion->mHeader.header[0], sizeof(mLoadingMotion->mHeader.header));
	mFp->LoadFile(&mLoadingMotion->mHeader.modelName[0], sizeof(mLoadingMotion->mHeader.modelName));
	mFp->LoadFile(&mLoadingMotion->mHeader.count);
}

void VMDLoader::LoadMotionDatas()
{
	mLoadingMotion->mMotions.resize(mLoadingMotion->mHeader.count);
	for (unsigned int i = 0; i < mLoadingMotion->mHeader.count; i++)
	{
		mFp->LoadFile(&mLoadingMotion->mMotions[i].boneName[0],sizeof(mLoadingMotion->mMotions[i].boneName));
		mFp->LoadFile(&mLoadingMotion->mMotions[i].frameNo);
		mFp->LoadFile(&mLoadingMotion->mMotions[i].location);
		mFp->LoadFile(&mLoadingMotion->mMotions[i].quoternion);
		mFp->LoadFile(&mLoadingMotion->mMotions[i].interpolation[0],sizeof(mLoadingMotion->mMotions[i].interpolation));
	}
}

void VMDLoader::CreatePoses()
{
	for (auto& m : mLoadingMotion->mMotions)
	{
		std::string boneName = m.boneName;
		VMDPose p;
		p.frameNo = m.frameNo;
		p.quoternion = m.quoternion;
		mLoadingMotion->mPoses[boneName].push_back(p);
	}

	for (auto& p : mLoadingMotion->mPoses)
	{
		std::sort(p.second.begin(), p.second.end(), [&](const VMDPose& lval, const VMDPose& rval) {return lval.frameNo < rval.frameNo; });
	}
}