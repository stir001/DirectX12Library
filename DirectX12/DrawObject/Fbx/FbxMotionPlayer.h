#pragma once
#include "FbxStructure.h"
#include "DirectX12/Animation/AnimationPlayer.h"
#include "FbxMotionData.h"
#include <memory>

class FbxMotionData;


class FbxMotionPlayer : public AnimationPlayer
{
public:
	FbxMotionPlayer(std::vector<Fbx::FbxSkeleton>& boneData, const std::vector<Fbx::FbxVertex>& vertices
		, std::vector<Fbx::FbxVertexElement>& vertexElements, std::vector<unsigned int>& skeletonIndices);
	~FbxMotionPlayer();

	void SetMotion(std::shared_ptr<FbxMotionData>& data, bool isLoop = true);
	void StopMotion() const;
	void ReStartMotion() const;
	void Update();
private:
	int mFrame;
	FbxMotionData mData;
	const std::vector<Fbx::FbxSkeleton>& mModelBones;
	std::vector<DirectX::XMFLOAT4X4> mCalMatrix;
	const std::vector<Fbx::FbxVertex>& mVertices;
	//std::vector<DirectX::XMFLOAT4X4> mVertexInitMatrix;
	std::vector<Fbx::FbxVertexElement>& mVertexElements;
	std::vector<std::vector<unsigned int>> mSkeletonTree;
	//std::vector<DirectX::XMFLOAT4X4> mInitMat;
	std::vector<DirectX::XMFLOAT4> mCalSkeletonPos;

	void UpdateCalMatrix();
	void UpdateVertexElementMatrix();
	void ApplyMotionData();
	void CreateSkeletonTree(std::vector<unsigned int>& skeletonIndices);
	void ApplyParentMatrix(std::vector<DirectX::XMFLOAT4X4>& matrix, std::vector<std::vector<unsigned int>>& tree);
	void ApplyParentMatrixRecursive(std::vector<DirectX::XMFLOAT4X4>& matrix, std::vector<std::vector<unsigned int>>& tree, unsigned int parentIndex);
	void CalSkeletonPos(std::vector<DirectX::XMFLOAT4>& pos, std::vector<DirectX::XMFLOAT4X4>& calMat, std::vector<std::vector<unsigned int>>& skeletonTree);
};

