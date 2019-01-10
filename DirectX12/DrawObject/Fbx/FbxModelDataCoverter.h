#pragma once
#include "FbxStructure.h"
#include <d3d12.h>
#include <wrl.h>

namespace Fbx
{
	struct FbxModelData;
	class FbxModel;
}

class FbxModelController;
struct FMDFileData;

class FbxModelDataConverter
{
public:
	FbxModelDataConverter();
	~FbxModelDataConverter();

	std::shared_ptr<Fbx::FbxModel> ConvertToFbxModel(std::shared_ptr<Fbx::FbxModelData> data);
	std::shared_ptr<Fbx::FbxModel> ConvertToFbxModel(FMDFileData& data);

private:
	std::weak_ptr<Fbx::FbxModel> mModel;
	std::string mRelativePath;
	std::weak_ptr<Fbx::FbxModelData> mConvertData;

	void ConvertIndex(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void ConvertVertex(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void ConvertTexture(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void ConvertSkeletons(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
};

