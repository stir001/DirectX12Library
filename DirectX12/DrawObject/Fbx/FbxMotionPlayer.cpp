#include "stdafx.h"
#include "FbxMotionPlayer.h"
#include "FbxMotionData.h"
#include "Util/XMFloatOperators.h"
#include "Animation/AnimationPlayerManager.h"
#include "Util/XMFloatOperators.h"

#include <algorithm>

FbxMotionPlayer::FbxMotionPlayer(std::vector<Fbx::FbxSkeleton>& bones, const std::vector<Fbx::FbxVertex>& vertices, std::vector<Fbx::FbxVertexElement>& vertexElements)
	:mModelBones(bones), mData{}, mFrame(0), mVertices(vertices), mVertexElements(vertexElements)
{
	mCalMatrix.resize(mModelBones.size());
}

FbxMotionPlayer::~FbxMotionPlayer()
{
	if (mAnimationId != -1)
	{
		AnimationPlayerManager::Instance().WaitSafeFree();
		AnimationPlayerManager::Instance().RemoveAnimation(mAnimationId);
	}
}

void FbxMotionPlayer::SetMotion(std::shared_ptr<FbxMotionData>& data, bool isLoop)
{
	if (data == nullptr)
	{
		return;
	}
	mData = *data;
	mIsLoop = isLoop;
	for (auto& itr : mData.mAnimData)
	{
		itr.currentDataIndex = 0;
	}
	mFrame = 0;
	mAnimationId = AnimationPlayerManager::Instance().RemoveAnimation(mAnimationId);
	mAnimationId = AnimationPlayerManager::Instance().SetAnimation(this);
	ApplyMotionData();
}

void FbxMotionPlayer::Update()
{
	if (mIsEnd && !mIsLoop)
	{
		return;
	}
	UpdateCalMatrix();
	UpdateVertexElementMatrix();
}

void FbxMotionPlayer::UpdateCalMatrix()
{
	if (mFrame == 0)
	{
		for (auto& itr : mData.mAnimData)
		{
			itr.currentDataIndex = 0;
			mIsEnd = false;
		}
	}

	for (unsigned int i = 0; i < mData.mAnimData.size(); ++i)
	{
		auto& anim = mData.mAnimData[i];
		auto& currentAnim = anim.frameData[anim.currentDataIndex];
		DirectX::XMStoreFloat4x4(&mCalMatrix[i], DirectX::XMMatrixIdentity());
		if (currentAnim.frame == (anim.frameData.back().frame))
		{
			DirectX::XMStoreFloat4x4(&mCalMatrix[i], currentAnim.matrix);
		}
		else
		{
			float t = (static_cast<float>(mFrame) - static_cast<float>(currentAnim.frame)) / (static_cast<float>(anim.frameData[anim.currentDataIndex + 1].frame - currentAnim.frame));
			DirectX::XMFLOAT4X4 rMatrix;
			DirectX::XMFLOAT4X4 baseMatrix;
			rMatrix = ConvertXMMATRIXToXMFloat4x4(currentAnim.matrix);
			baseMatrix = ConvertXMMATRIXToXMFloat4x4(anim.frameData[anim.currentDataIndex + 1].matrix);
			mCalMatrix[i] = rMatrix * (1.0f - t) + baseMatrix * t;
			if (mFrame == anim.frameData[anim.currentDataIndex + 1].frame)
			{
				++anim.currentDataIndex;
			}
		}
	}

	++mFrame;
	if (mFrame == mData.mMaxFrame)
	{
		mIsEnd = true;
	}
	mFrame %= mData.mMaxFrame;
}

void FbxMotionPlayer::UpdateVertexElementMatrix()
{
	DirectX::XMFLOAT4X4 vertexMatrix;
	DirectX::XMFLOAT4X4 initVertexMatrix;
	DirectX::XMFLOAT4X4 multiMatrix;
	std::string findBoneName;
	for (unsigned int i = 0; i < mVertices.size();++i)
	{
		vertexMatrix = {};
		initVertexMatrix = {};
		for (unsigned int j = 0; j < mVertices[i].boneIndex.size(); ++j)
		{
			int boneIndex = mVertices[i].boneIndex[j];
			float boneweight = mVertices[i].boneWeight[j];
			vertexMatrix += mCalMatrix[boneIndex] * boneweight;
			//initVertexMatrix += mModelBones[boneIndex].initMatrix * boneweight;
		}
		multiMatrix = initVertexMatrix * vertexMatrix;
		mVertexElements[i].pos = mVertices[i].pos * multiMatrix;
		multiMatrix._41 = 0;
		multiMatrix._42 = 0;
		multiMatrix._43 = 0;
		mVertexElements[i].normal = mVertices[i].normal * multiMatrix;
	}
}

void FbxMotionPlayer::ApplyMotionData()
{
	for (unsigned int i = 0; i < mModelBones.size(); ++i)
	{
		for (unsigned int j = i; j < mData.mAnimData.size(); ++j)
		{
			if (mModelBones[i].name == mData.mAnimData[j].boneName)
			{
				Fbx::BoneMatrixData tmp = {};
				tmp = mData.mAnimData[j];
				mData.mAnimData[j] = mData.mAnimData[i];
				mData.mAnimData[i] = tmp;
			}
		}
	}
}

void FbxMotionPlayer::StopMotion() const
{
	AnimationPlayerManager::Instance().StopAnimation(mAnimationId);
}

void FbxMotionPlayer::ReStartMotion() const
{
	AnimationPlayerManager::Instance().ReStartAnimation(mAnimationId);
}
