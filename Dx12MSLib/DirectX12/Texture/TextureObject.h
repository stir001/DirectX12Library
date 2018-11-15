#pragma once
#include <memory>
#include <string>
#include <d3d12.h>
#include <wrl.h>

class ShaderResourceObject;
namespace DirectX {
	class ScratchImage;
}

class TextureObject
{
	friend class TextureLoader;
public:
	TextureObject();
	~TextureObject();

	int GetWidth() const;
	int GetHeight() const;
	std::shared_ptr<ShaderResourceObject> GetShaderResource() const;
	const std::string& GetTextureName() const;
	const std::string& GetFilePath() const;
	float GetGamma() const;
private:
	std::string mFilepath;
	std::string mTextureName;
	std::shared_ptr<ShaderResourceObject> mShaderResource;
	std::shared_ptr<DirectX::ScratchImage> mImageData;
	D3D12_SUBRESOURCE_DATA mSubresource;
	float mGamma;
};

