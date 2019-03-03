#include "stdafx.h"
#include "VMDPlayer.h"
#include "VMDAnimation.h"
#include "DrawObject/PMD/PMDModel.h"
#include "Buffer/ConstantBufferObject.h"
#include "Animation/AnimationPlayerUpdater.h"
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
		AnimationPlayerUpdater::Instance().WaitAnimation();
	}
}

void VMDPlayer::Update()
{
	(this->*mUpdate)();
}

void VMDPlayer::SetVMD(std::shared_ptr<VMDAnimation>& vmd)
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
	ApplyRootLocation();
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

void VMDPlayer::ApplyRootLocation()
{
	auto rootItr = (*mCurrentPoses.begin());
	DirectX::XMMATRIX mats = DirectX::XMMatrixTranslation(rootItr->location.x, rootItr->location.y, rootItr->location.z);
	DirectX::XMMATRIX matg = DirectX::XMMatrixTranslation((rootItr + 1)->location.x, (rootItr + 1)->location.y, (rootItr + 1)->location.z);
	DirectX::XMFLOAT4X4 f44s = ConvertXMMATRIXToXMFloat4x4(mats);
	DirectX::XMFLOAT4X4 f44g = ConvertXMMATRIXToXMFloat4x4(matg);
	float t = static_cast<float>(mFrame * 0.5f - rootItr->frameNo) / static_cast<float>((rootItr + 1)->frameNo - rootItr->frameNo);
	DirectX::XMFLOAT4X4 mat = f44g * t + f44s * (1.0f - t);
	for (auto& currentMat : mCurrentBoneMatrix)
	{
		currentMat *= mat;
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
	mAnimationId = AnimationPlayerUpdater::Instance().SetAnimation(this);
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
	AnimationPlayerUpdater::Instance().RemoveAnimation(mAnimationId);
	mAnimationId = -1;
}

void VMDPlayer::WriteBoneMatrix(std::shared_ptr<ConstantBufferObject>& matrixBuffer)
{
	matrixBuffer->WriteBuffer(&mCurrentBoneMatrix[0], static_cast<unsigned int>(mCurrentBoneMatrix.size() * sizeof(mCurrentBoneMatrix[0])));
}