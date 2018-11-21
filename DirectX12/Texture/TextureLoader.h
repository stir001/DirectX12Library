#pragma once

#include <memory>
#include <map>
#include <string>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>

struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;
struct ID3D12DescriptorHeap;
class TextureObject;
class TextureMgr;
class Dx12DescriptorHeapObject;

namespace DirectX
{
	struct TexMetadata;
	class ScratchImage;
}

class TextureLoader
{
public:
	~TextureLoader();

	static TextureLoader& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new TextureLoader();
		}

		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	void Release(const std::string& filePath);

	std::shared_ptr<TextureObject> LoadTexture(const std::string& filepath);

	std::shared_ptr<TextureObject> CreateSingleColorTexture(const DirectX::XMFLOAT4& color);
	std::shared_ptr<TextureObject> CreateSingleColorTexture(const float color);
private:
	TextureLoader();
	TextureLoader(const TextureLoader&);
	TextureLoader(const TextureLoader&&);
	TextureLoader& operator=(TextureLoader&);
	TextureLoader& operator=(TextureLoader&&);

	static TextureLoader* mInstance;

	std::map<std::string, std::shared_ptr<TextureObject>> mTextures;

	void CreateTexWriteToSubRrsource(std::shared_ptr<TextureObject>& inTex);
	void CreateTexUpdateSubResources(std::shared_ptr<TextureObject>& inTex);
	void CreateNullTexture(std::shared_ptr<TextureObject>& inTex);

	std::string GetTextureName(const std::wstring& filePath);
	bool IsUseGamma(DXGI_FORMAT fomat);
	HRESULT LoadWIC(const std::wstring& path, DirectX::TexMetadata& metaData, DirectX::ScratchImage& scracgImage);
	HRESULT LoadTGA(const std::wstring& path, DirectX::TexMetadata& metaData, DirectX::ScratchImage& scracgImage);
	HRESULT LoadDDS(const std::wstring& path, DirectX::TexMetadata& metaData, DirectX::ScratchImage& scracgImage);
};

