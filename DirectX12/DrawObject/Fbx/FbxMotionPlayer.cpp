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
	mInverseMatrix.resize(boneNum);
	mQuoternionMatrix.resize(boneNum);
	for (unsigned int i = 0; i < boneNum; ++i)
	{
		//mInverseMatrix[i] = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixTranslation(-mModelBones[i].pos.x, -mModelBones[i].pos.y, -mModelBones[i].pos.z));
		mInverseMatrix[i] = InverseXMFloat4x4(mModelBones[i].localMatrix);
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

	ApplyParentMatrixRecursive(mCalMatrix, mSkeletonTree, 0);
	UpdateQuoternionMatrix();
	UpdateSkeletonPos();
	//ApplyParentMatrixRecursive(mQuoternionMatrix, mSkeletonTree, 0);
	//ApplyInverseMatrix();

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
	for (unsigned int i = 0; i < mVertices.size();++i)
	{
		vertexMatrix = {};
		auto& vert = mVertices[i];
		for (unsigned int j = 0; j < vert.boneIndex.size(); ++j)
		{
			//vertexMatrix += mCalMatrix[vert.boneIndex[j]] * vert.boneWeight[j];
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
				break;
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
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	std::vector<DirectX::XMFLOAT4> calpos(boneNum);
	for (int i = 0; i < mModelBones.size(); ++i)
	{
		//calpos[i] = DirectX::XMFLOAT4(0, 0, 0, 1) * mCalMatrix[i];
		/*calpos[i] = mModelBones[i].pos * mCalMatrix[i];*/
		calpos[i] = mModelBones[i].pos * mQuoternionMatrix[i];
	}
	mSkeletonPosBuffer->WriteBuffer(calpos.data(), static_cast<unsigned int>(sizeof(DirectX::XMFLOAT4) * mModelBones.size()));
}

void FbxMotionPlayer::UpdateQuoternionMatrix()
{
	unsigned int boneNum = static_cast<unsigned int>(mModelBones.size());
	std::vector<DirectX::XMFLOAT4> initpos(boneNum);
	std::vector<DirectX::XMFLOAT4> calpos(boneNum);
	for (int i = 0; i < mModelBones.size(); ++i)
	{
		calpos[i] = DirectX::XMFLOAT4(0, 0, 0, 1) * mCalMatrix[i];
		DirectX::XMFLOAT3 cPos = NormalizeXMFloat3({ calpos[i].x, calpos[i].y, calpos[i].z });
		initpos[i] = DirectX::XMFLOAT4(0, 0, 0, 1) * mModelBones[i].globalMatrix;
		DirectX::XMFLOAT3 iPos = NormalizeXMFloat3({ initpos[i].x, initpos[i].y, initpos[i].z });

		float cos = DotXMFloat3(iPos, cPos);
		if (1.0f - abs(cos) <= 0.0001f)
		{
			mQuoternionMatrix[i] = IdentityXMFloat4x4();
		}
		else
		{
			DirectX::XMFLOAT3 axis = NormalizeXMFloat3(CrossXMFloat3(iPos, cPos));
			auto quot = CreateQuoternion(axis, DirectX::XMConvertToDegrees(acos(cos)));
			auto transform = DirectX::XMMatrixRotationQuaternion(quot);
			mQuoternionMatrix[i] = ConvertXMMATRIXToXMFloat4x4(transform);
		}
	}
}

void FbxMotionPlayer::ApplyInverseMatrix()
{
	for (unsigned int i = 0; static_cast<unsigned int>(i < mCalMatrix.size()); ++i)
	{
		mCalMatrix[i] = mInverseMatrix[i] * mCalMatrix[i];
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
