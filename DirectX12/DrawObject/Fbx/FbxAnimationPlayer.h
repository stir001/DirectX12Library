#pragma once
#include "DirectX12/Animation/AnimationPlayer.h"
#include "FbxStructure.h"
#include "FbxAnimationData.h"
#include <memory>

class FbxAnimationData;
class VertexBufferObject;


class FbxAnimationPlayer : public AnimationPlayer
{
public:
	FbxAnimationPlayer(std::vector<Fbx::FbxSkeleton>& boneData, const std::vector<Fbx::FbxVertex>& vertices
		, std::vector<Fbx::FbxVertexElement>& vertexElements, std::vector<unsigned int>& skeletonIndices
		, std::shared_ptr<VertexBufferObject> skeletonPosBuffer);
	~FbxAnimationPlayer();

	void SetAnimation(std::shared_ptr<FbxAnimationData>& data, bool isLoop = true);
	void StopAnimation() const;
	void ReStartAnimation() const;
	void Update();
private:
	int mFrame;
	FbxAnimationData mData;
	const std::vector<Fbx::FbxSkeleton>& mModelBones;
	std::vector<DirectX::XMFLOAT4X4> mCalMatrix;
	const std::vector<Fbx::FbxVertex>& mVertices;
	std::vector<Fbx::FbxVertexElement>& mVertexElements;
	std::vector<std::vector<unsigned int>> mSkeletonTree;
	std::vector<DirectX::XMFLOAT4X4> mInverseMatrix;
	std::vector<DirectX::XMFLOAT4X4> mQuoternionMatrix;
	std::shared_ptr<VertexBufferObject> mSkeletonPosBuffer;
	std::vector<DirectX::XMFLOAT4> mSkeletonTailPos;
	std::vector<DirectX::XMFLOAT4> mSkeletonHeadPos;
	std::shared_ptr<FbxAnimationData> mNextData;

	void UpdateCalMatrix();
	void UpdateVertexElementMatrix();
	void ApplyAnimationData();
	void CreateSkeletonTree(std::vector<unsigned int>& skeletonIndices);
	void ApplyParentMatrixRecursive(std::vector<DirectX::XMFLOAT4X4>& matrix, std::vector<std::vector<unsigned int>>& tree, unsigned int parentIndex);
	void UpdateSkeletonPos();
	void UpdateQuoternionMatrix();
	void CalTailPos();
	void ChangeNextAnimationData();
};

