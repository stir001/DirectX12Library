#pragma once
#include <vector>
#include <string>

namespace Fbx
{
	struct SkeletonAnimationData;
}
class FbxMotionData;

class FbxMotionConverter
{
public:
	FbxMotionConverter();
	~FbxMotionConverter();

	FbxMotionData* ConvertMotionData(const std::vector<Fbx::SkeletonAnimationData>& data, long long int maxframe, const std::string& animatiomPath);

private:
	FbxMotionData* mMotion;
};