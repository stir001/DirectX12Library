#pragma once
#include <vector>
#include <DirectXMath.h>

namespace fbxsdk
{
	class FbxAnimCurve;
}

namespace Fbx {
	struct AnimKeyData
	{
		float value;
		long long int time;
	};

	struct AnimCurveData
	{
		fbxsdk::FbxAnimCurve* curve;
		std::vector<AnimKeyData> keys;
	};

	struct AnimationData
	{
		std::string skeletonName;
		AnimCurveData translationX;
		AnimCurveData translationY;
		AnimCurveData translationZ;

		AnimCurveData rotationX;
		AnimCurveData rotationY;
		AnimCurveData rotationZ;

		AnimCurveData scaleX;
		AnimCurveData scaleY;
		AnimCurveData scaleZ;
	};

	struct AnimationMatrix
	{
		int frame;
		DirectX::XMMATRIX matrix;
	};

	struct SkeletonAnimationData
	{
		std::string skeletonName;
		std::vector<AnimationMatrix> animMatrix;
	};
};