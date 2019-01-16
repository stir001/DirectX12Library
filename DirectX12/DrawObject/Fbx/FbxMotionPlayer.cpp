#include "stdafx.h"
#include "FbxMotionPlayer.h"
#include "FbxMotionData.h"
#include "Util/XMFloatOperators.h"
#include "Animation/AnimationPlayerManager.h"
#include "Util/XMFloatOperators.h"

#include <algorithm>

FbxMotionPlayer::FbxMotionPlayer(std::vector<Fbx::FbxSkeleton>& bones, const std::vector<Fbx::FbxVertex>& vertices
	, std::vector<Fbx::FbxVertexElement>& vertexElements, std::vector<unsigned int>& skeletonIndices)
	:mModelBones(bones), mData{}, mFrame(0), mVertices(vertices), mVertexElements(vertexElements)
{
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	mCalMatrix.resize(boneNum);
	mCalSkeletonPos.resize(boneNum);
	mPoseMatrix.resize(boneNum);
	CreateSkeletonTree(skeletonIndices);
}

FbxMotionPlayer::~FbxMotionPlayer()
{
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

	for (unsigned int i = 0; i < mData.mAnimData.size() && i < mCalMatrix.size(); ++i)
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

	ApplyParentMatrixRecursive(mCalMatrix, mSkeletonTree, 0);
	CalSkeletonPos(mCalSkeletonPos, mCalMatrix);

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
	DirectX::XMFLOAT4X4 multiMatrix;
	std::string findBoneName;
	for (unsigned int i = 0; i < mVertices.size();++i)
	{
		vertexMatrix = {};
		auto& vert = mVertices[i];
		for (unsigned int j = 0; j < vert.boneIndex.size(); ++j)
		{
			vertexMatrix += mCalMatrix[vert.boneIndex[j]] * vert.boneWeight[j];
		}
		multiMatrix = vertexMatrix;
		mVertexElements[i].pos = vert.pos/* * mVertexInitMatrix[i]*/ * multiMatrix;
		multiMatrix._41 = 0;
		multiMatrix._42 = 0;
		multiMatrix._43 = 0;
		mVertexElements[i].normal = vert.normal * multiMatrix;
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

void FbxMotionPlayer::CreateSkeletonTree(std::vector<unsigned int>& skeletonIndices)
{
	mSkeletonTree.resize(mModelBones.size());
	unsigned int indicesSize = static_cast<unsigned int>(skeletonIndices.size());
	for (unsigned int i = 0; i < indicesSize; i += 2)
	{
		mSkeletonTree[skeletonIndices[i]].push_back(skeletonIndices[i + 1]);
	}
}

void FbxMotionPlayer::ApplyParentMatrixRecursive(std::vector<DirectX::XMFLOAT4X4>& matrix, std::vector<std::vector<unsigned int>>& tree, unsigned int parentIndex)
{
	for (unsigned int i = 0; i < tree[parentIndex].size(); ++i)
	{
		unsigned int childIndex = tree[parentIndex][i];
		ApplyParentMatrixRecursive(matrix, tree, childIndex);
		matrix[childIndex] = matrix[childIndex] * matrix[parentIndex];
		//matrix[childIndex] = matrix[parentIndex] * matrix[childIndex];
	}
}

void FbxMotionPlayer::CalSkeletonPos(std::vector<DirectX::XMFLOAT4>& skeletonPos, std::vector<DirectX::XMFLOAT4X4>& calMat)
{
	DirectX::XMFLOAT4 origin(0, 0, 0, 1);
	unsigned int size = static_cast<unsigned int>(skeletonPos.size());
	for (unsigned int i = 0; i < size; ++i)
	{
		skeletonPos[i] = origin * calMat[i];
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
