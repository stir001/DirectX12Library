#pragma once
#include "DirectX12/Animation/AnimationPlayer.h"
#include "FbxStructure.h"
#include "FbxMotionData.h"
#include <memory>

class FbxMotionData;
class VertexBufferObject;


class FbxMotionPlayer : public AnimationPlayer
{
public:
	FbxMotionPlayer(std::vector<Fbx::FbxSkeleton>& boneData, const std::vector<Fbx::FbxVertex>& vertices
		, std::vector<Fbx::FbxVertexElement>& vertexElements, std::vector<unsigned int>& skeletonIndices
		, std::shared_ptr<VertexBufferObject> skeletonPosBuffer);
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
	std::vector<Fbx::FbxVertexElement>& mVertexElements;
	std::vector<std::vector<unsigned int>> mSkeletonTree;
	std::vector<DirectX::XMFLOAT4X4> mInitMatrix;
	std::shared_ptr<VertexBufferObject> mSkeletonPosBuffer;


	void UpdateCalMatrix();
	void UpdateVertexElementMatrix();
	void ApplyMotionData();
	void CreateSkeletonTree(std::vector<unsigned int>& skeletonIndices);
	void ApplyParentMatrixRecursive(std::vector<DirectX::XMFLOAT4X4>& matrix, std::vector<std::vector<unsigned int>>& tree, unsigned int parentIndex);
	void UpdateSkeletonPos();

};

