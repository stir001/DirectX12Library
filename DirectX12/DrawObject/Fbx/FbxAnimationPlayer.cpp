#include "stdafx.h"
#include "FbxAnimationPlayer.h"
#include "FbxAnimationData.h"
#include "Util/XMFloatOperators.h"
#include "Animation/AnimationPlayerUpdater.h"
#include "Util/XMFloatOperators.h"
#include "Buffer/VertexBufferObject.h"

#include <algorithm>

FbxAnimationPlayer::FbxAnimationPlayer(std::vector<Fbx::FbxSkeleton>& bones, const std::vector<Fbx::FbxVertex>& vertices
	, std::vector<Fbx::FbxVertexElement>& vertexElements, std::vector<unsigned int>& skeletonIndices
	, std::shared_ptr<VertexBufferObject> skeletonPosBuffer)
	: mModelBones(bones), mData{}, mFrame(0)
	, mVertices(vertices), mVertexElements(vertexElements)
	, mSkeletonPosBuffer(skeletonPosBuffer)
	, mNextData(nullptr)
{
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	mCalMatrix.resize(boneNum);
	mInverseMatrix.resize(boneNum);
	mQuoternionMatrix.resize(boneNum);
	mSkeletonTailPos.resize(boneNum);
	mSkeletonHeadPos.resize(boneNum);
	for (unsigned int i = 0; i < boneNum; ++i)
	{
		mInverseMatrix[i] = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixTranslation(-mModelBones[i].pos.x, -mModelBones[i].pos.y, -mModelBones[i].pos.z));
	}
	CreateSkeletonTree(skeletonIndices);
}

FbxAnimationPlayer::~FbxAnimationPlayer()
{
	if (mAnimationId != -1)
	{
		mAnimationId = AnimationPlayerUpdater::Instance().RemoveAnimation(mAnimationId);
	}
}

void FbxAnimationPlayer::SetAnimation(std::shared_ptr<FbxAnimationData>& data, bool isLoop)
{
	if (data == nullptr)
	{
		return;
	}
	AnimationPlayerUpdater::Instance().WaitAnimation();
	mNextData = data;
	mIsLoop = isLoop;
	mAnimationId = AnimationPlayerUpdater::Instance().RemoveAnimation(mAnimationId);
	mAnimationId = AnimationPlayerUpdater::Instance().SetAnimation(this);
}

void FbxAnimationPlayer::Update()
{
	if (mIsEnd && !mIsLoop)
	{
		return;
	}
	if (mNextData)
	{
		ChangeNextAnimationData();
	}
	UpdateCalMatrix();
	ApplyParentMatrixRecursive(mCalMatrix, mSkeletonTree, 0);
	CalTailPos();
	UpdateQuoternionMatrix();
	UpdateVertexElementMatrix();
}

void FbxAnimationPlayer::UpdateCalMatrix()
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
			mCalMatrix[i] = currentAnim.matrix;
		}
		else
		{
			float t = (static_cast<float>(mFrame) - static_cast<float>(currentAnim.frame)) / (static_cast<float>(anim.frameData[anim.currentDataIndex + 1].frame - currentAnim.frame));			
			mCalMatrix[i] = (currentAnim.matrix * (1.0f - t) + anim.frameData[anim.currentDataIndex + 1].matrix * t);
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

void FbxAnimationPlayer::UpdateVertexElementMatrix()
{
	DirectX::XMFLOAT4X4 vertexMatrix;
	DirectX::XMFLOAT4X4 multiMatrix;
	for (unsigned int i = 0; i < mVertices.size();++i)
	{
		vertexMatrix = {};
		auto& vert = mVertices[i];
		for (unsigned int j = 0; j < vert.boneIndex.size(); ++j)
		{
			vertexMatrix += mQuoternionMatrix[vert.boneIndex[j]] * vert.boneWeight[j];
		}
		multiMatrix = vertexMatrix;
		mVertexElements[i].pos = vert.pos * multiMatrix;
		multiMatrix._41 = 0;
		multiMatrix._42 = 0;
		multiMatrix._43 = 0;
		mVertexElements[i].normal = vert.normal * multiMatrix;
	}
}

void FbxAnimationPlayer::ApplyAnimationData()
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
				break;
			}
		}
	}
}

void FbxAnimationPlayer::CreateSkeletonTree(std::vector<unsigned int>& skeletonIndices)
{
	mSkeletonTree.resize(mModelBones.size());
	unsigned int indicesSize = static_cast<unsigned int>(skeletonIndices.size());
	for (unsigned int i = 0; i < indicesSize; i += 2)
	{
		mSkeletonTree[skeletonIndices[i]].push_back(skeletonIndices[i + 1]);
	}
}

void FbxAnimationPlayer::ApplyParentMatrixRecursive(std::vector<DirectX::XMFLOAT4X4>& matrix, std::vector<std::vector<unsigned int>>& tree, unsigned int parentIndex)
{
	for (unsigned int i = 0; i < tree[parentIndex].size(); ++i)
	{
		unsigned int childIndex = tree[parentIndex][i];
		matrix[childIndex] = matrix[childIndex] * matrix[parentIndex];
		ApplyParentMatrixRecursive(matrix, tree, childIndex);
	}
}

void FbxAnimationPlayer::UpdateSkeletonPos()
{
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	std::vector<DirectX::XMFLOAT4> calpos(boneNum);
	for (int i = 0; i < mModelBones.size(); ++i)
	{
		calpos[i] = DirectX::XMFLOAT4(0, 0, 0, 1) * mCalMatrix[i];
	}
	mSkeletonPosBuffer->WriteBuffer(calpos.data(), static_cast<unsigned int>(sizeof(DirectX::XMFLOAT4) * mModelBones.size()));
}

void FbxAnimationPlayer::UpdateQuoternionMatrix()
{
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	std::vector<DirectX::XMFLOAT4> initPos(boneNum);
	std::vector<DirectX::XMFLOAT4> calPos(boneNum);
	for (int i = 0; i < mModelBones.size(); ++i)
	{
		initPos[i] = mModelBones[i].tailPos - mModelBones[i].pos;
		DirectX::XMFLOAT3 iPos = NormalizeXMFloat3({ initPos[i].x, initPos[i].y, initPos[i].z });
		calPos[i] = mSkeletonTailPos[i] - mSkeletonHeadPos[i];
		DirectX::XMFLOAT3 cPos = NormalizeXMFloat3({ calPos[i].x, calPos[i].y, calPos[i].z });

		auto tranlateMat = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixTranslation(mSkeletonHeadPos[i].x, mSkeletonHeadPos[i].y, mSkeletonHeadPos[i].z));

		float cos = DotXMFloat3(iPos, cPos);
		if (1.0f - abs(cos) <= 0.0001f)
		{
			mQuoternionMatrix[i] = mInverseMatrix[i] * IdentityXMFloat4x4() * tranlateMat;
		}
		else
		{
			DirectX::XMFLOAT3 axis = NormalizeXMFloat3(CrossXMFloat3(iPos, cPos));
			auto transform = CreateQuoternion(axis, acos(cos));
			mQuoternionMatrix[i] = mInverseMatrix[i] * transform * tranlateMat;
		}
	}
}

void FbxAnimationPlayer::CalTailPos()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(mSkeletonHeadPos.size()); ++i)
	{
		mSkeletonHeadPos[i] = (DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) * mCalMatrix[i]);
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(mSkeletonTree.size()); ++i)
	{
		mSkeletonTailPos[i] = {0, 0, 0, 1.0f};
		for (auto& child : mSkeletonTree[i])
		{
			mSkeletonTailPos[i] += mSkeletonHeadPos[child] / static_cast<float>(mSkeletonTree[i].size());
		}
	}
}

void FbxAnimationPlayer::ChangeNextAnimationData()
{
	mData = *mNextData;
	mNextData = nullptr;
	for (auto& itr : mData.mAnimData)
	{
		itr.currentDataIndex = 0;
	}
	mFrame = 0;
	ApplyAnimationData();
}

void FbxAnimationPlayer::StopAnimation() const
{
	AnimationPlayerUpdater::Instance().StopAnimation(mAnimationId);
}

void FbxAnimationPlayer::ReStartAnimation() const
{
	AnimationPlayerUpdater::Instance().ReStartAnimation(mAnimationId);
}
