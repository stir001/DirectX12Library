#pragma once
#include <vector>
#include <string>

namespace Fbx
{
	struct SkeletonAnimationData;
}
class FbxAnimationData;

class FbxAnimationConverter
{
public:
	FbxAnimationConverter();
	~FbxAnimationConverter();

	FbxAnimationData* ConvertAnimationData(const std::vector<Fbx::SkeletonAnimationData>& data, long long int maxframe, const std::string& animatiomPath);

private:
	FbxAnimationData* mAnimation;
};