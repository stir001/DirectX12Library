#include "stdafx.h"
#include "FbxMotionPlayer.h"
#include "FbxMotionData.h"
#include "Util/XMFloatOperators.h"
#include "Animation/AnimationPlayerManager.h"
#include "Util/XMFloatOperators.h"
#include "Buffer/VertexBufferObject.h"

#include <algorithm>

FbxMotionPlayer::FbxMotionPlayer(std::vector<Fbx::FbxSkeleton>& bones, const std::vector<Fbx::FbxVertex>& vertices
	, std::vector<Fbx::FbxVertexElement>& vertexElements, std::vector<unsigned int>& skeletonIndices
	, std::shared_ptr<VertexBufferObject> skeletonPosBuffer)
	: mModelBones(bones), mData{}, mFrame(0)
	, mVertices(vertices), mVertexElements(vertexElements)
	, mSkeletonPosBuffer(skeletonPosBuffer)
{
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	mCalMatrix.resize(boneNum);
	mInitMatrix.resize(boneNum);
	for (unsigned int i = 0; i < boneNum; ++i)
	{
		mInitMatrix[i] = InverseXMFloat4x4(mModelBones[i].initMatrix);
	}
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
	//UpdateSkeletonPos();
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
		mCalMatrix[i] = IdentityXMFloat4x4();
		if (currentAnim.frame == (anim.frameData.back().frame))
		{
			mCalMatrix[i] = mInitMatrix[i] * currentAnim.matrix;
		}
		else
		{
			float t = (static_cast<float>(mFrame) - static_cast<float>(currentAnim.frame)) / (static_cast<float>(anim.frameData[anim.currentDataIndex + 1].frame - currentAnim.frame));			
			mCalMatrix[i] = mInitMatrix[i] * (currentAnim.matrix * (1.0f - t) + anim.frameData[anim.currentDataIndex + 1].matrix * t);
			if (mFrame == anim.frameData[anim.currentDataIndex + 1].frame)
			{
				++anim.currentDataIndex;
			}
		}
	}

	ApplyParentMatrixRecursive(mCalMatrix, mSkeletonTree, 0);

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
		mVertexElements[i].pos = vert.pos * multiMatrix;
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
		matrix[childIndex] = matrix[childIndex] * matrix[parentIndex];
		ApplyParentMatrixRecursive(matrix, tree, childIndex);
		//matrix[childIndex] = matrix[parentIndex] * matrix[childIndex];
	}
}

void FbxMotionPlayer::UpdateSkeletonPos()
{
	std::vector<DirectX::XMFLOAT4> pos(mModelBones.size());
	for (int i = 0; i < mModelBones.size(); ++i)
	{
		pos[i] = DirectX::XMFLOAT4(0, 0, 0, 1) * mCalMatrix[i];
	}
	mSkeletonPosBuffer->WriteBuffer(pos.data(), sizeof(DirectX::XMFLOAT4) * mModelBones.size());
}

void FbxMotionPlayer::StopMotion() const
{
	AnimationPlayerManager::Instance().StopAnimation(mAnimationId);
}

void FbxMotionPlayer::ReStartMotion() const
{
	AnimationPlayerManager::Instance().ReStartAnimation(mAnimationId);
}
