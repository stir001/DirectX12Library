#include "stdafx.h"
#include "VMDPlayer.h"
#include "VMDMotion.h"
#include "DrawObject/PMD/PMDModel.h"
#include "Buffer/ConstantBufferObject.h"
#include "Animation/AnimationPlayerManager.h"
#include "Util//XMFloatOperators.h"

#include <algorithm>

using namespace DirectX;

VMDPlayer::VMDPlayer(std::vector<PMDBoneData>& bDatas, BoneTree& node, std::vector<DirectX::XMFLOAT4X4>& boneMat, std::shared_ptr<ConstantBufferObject>& boneConstantBuffer)
	:mBoneDatas(bDatas), mBoneNode(node), mFrame(0)
	, mUpdate(&VMDPlayer::StopUpdate), mEndCheck(&VMDPlayer::EndCheck), mCurrentBoneMatrix(boneMat)
	, mBoneConstantBuffer(boneConstantBuffer)
{
}

VMDPlayer::~VMDPlayer()
{
	if (mAnimationId != -1)
	{
		AnimationPlayerManager::Instance().WaitSafeFree();
	}
}

void VMDPlayer::Update()
{
	(this->*mUpdate)();
}

void VMDPlayer::SetVMD(std::shared_ptr<VMDMotion>& vmd)
{
	mPoses = &vmd->mPoses;
	mMaxFrame = 0;
	for (auto& p : *mPoses)
	{
		if (p.second.back().frameNo > mMaxFrame)
		{
			mMaxFrame = p.second.back().frameNo;
		}
		XMVECTOR v = XMLoadFloat4(&p.second.front().quoternion);
		XMMATRIX q = XMMatrixRotationQuaternion(v);
		VMDBoneRotation(p.first, q);

	}
	WriteBoneMatrix(mBoneConstantBuffer);
	PoseSet();
}

void VMDPlayer::PlayingUpdate()
{
	DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 iMat;
	DirectX::XMStoreFloat4x4(&iMat, identity);
	std::fill(mCurrentBoneMatrix.begin(), mCurrentBoneMatrix.end(), iMat);

	auto posesItr = mPoses->begin();

	for (auto& p : mCurrentPoses)
	{
		if ((p + 1) != posesItr->second.end() && (p + 1)->frameNo < mFrame * 0.5f)
		{
			++p;
		}
		if (p == (posesItr->second.end() - 1))
		{
			
			DirectX::XMVECTOR v = DirectX::XMLoadFloat4(&p->quoternion);
			DirectX::XMMATRIX q = DirectX::XMMatrixRotationQuaternion(v);
			VMDBoneRotation(posesItr->first, q);
		}
		else
		{
			float t = (static_cast<float>(mFrame) * 0.5f - static_cast<float>(p->frameNo)) / static_cast<float>((p + 1)->frameNo - p->frameNo);
			XMVECTOR q = DirectX::XMQuaternionSlerp(DirectX::XMLoadFloat4(&p->quoternion), DirectX::XMLoadFloat4(&(p + 1)->quoternion), t);
			DirectX::XMMATRIX mat = DirectX::XMMatrixRotationQuaternion(q);
			VMDBoneRotation(posesItr->first, mat);
		}
		++posesItr;
	}
	VMDBoneChildRotation(mCurrentBoneMatrix[0], 0);
	WriteBoneMatrix(mBoneConstantBuffer);
	++mFrame;
	if (mIsLoop && mFrame == mMaxFrame * 2)
	{
		PoseSet();
		mFrame = 0;
	}
	(this->*mEndCheck)();
}

void VMDPlayer::StopUpdate()
{

}

void VMDPlayer::PoseSet()
{
	unsigned int size = static_cast<unsigned int>(mPoses->size());
	mCurrentPoses.resize(size);
	auto posesItr = mPoses->begin();
	for (unsigned int i = 0; i < size; ++i, ++posesItr)
	{
		mCurrentPoses[i] = posesItr->second.begin();
	}
}

void VMDPlayer::VMDBoneRotation(const std::string& boneName, const XMMATRIX& boneRotaMatrix)
{
	const PMDBoneData* data = nullptr;
	unsigned int i = 0;
	for (; i < mBoneDatas.size(); i++)
	{
		if (mBoneDatas[i].boneName == boneName)
		{
			data = &mBoneDatas[i];
			break;
		}
	}
	if (data == nullptr) return;

	XMVECTOR offsetvec = XMLoadFloat3(&data->pos);
	XMMATRIX boneMat = DirectX::XMMatrixTranslationFromVector(-offsetvec);
	boneMat *= boneRotaMatrix;
	boneMat *= DirectX::XMMatrixTranslationFromVector(offsetvec);
	DirectX::XMStoreFloat4x4(&mCurrentBoneMatrix[i] ,boneMat * DirectX::XMLoadFloat4x4(&mCurrentBoneMatrix[i]));
}

void VMDPlayer::VMDBoneChildRotation(const XMFLOAT4X4& parentBoneMatrix, int parentIndex)
{
	for (auto& childIndex : mBoneNode.node[parentIndex])
	{
		mCurrentBoneMatrix[childIndex] *= parentBoneMatrix;
		VMDBoneChildRotation(mCurrentBoneMatrix[childIndex], childIndex);
	}
}

void VMDPlayer::Play(bool flag)
{
	mIsLoop = flag;
	mUpdate = &VMDPlayer::PlayingUpdate;
	mAnimationId = AnimationPlayerManager::Instance().SetAnimation(this);
	if (mIsLoop)
	{
		mEndCheck = &VMDPlayer::NonCheck;
	}
	else
	{
		mEndCheck = &VMDPlayer::EndCheck;
	}
}

void VMDPlayer::EndCheck()
{
	if (mFrame == 0) Stop();
}

void VMDPlayer::NonCheck()
{
}

void VMDPlayer::Stop()
{
	mUpdate = &VMDPlayer::StopUpdate;
	AnimationPlayerManager::Instance().RemoveAnimation(mAnimationId);
	mAnimationId = -1;
}

void VMDPlayer::WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer)
{
	matrixBuffer->WriteBuffer(&mCurrentBoneMatrix[0], static_cast<unsigned int>(mCurrentBoneMatrix.size() * sizeof(mCurrentBoneMatrix[0])));
}