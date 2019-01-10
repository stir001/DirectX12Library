#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <string>

class RootSignatureObject;
class PipelineStateObject;
class TextureObject;
class VertexBufferObject;
class IndexBufferObject;
class Dx12CommandList;
class Dx12DescriptorHeapObject;
class CameraHolder;
class ConstantBufferObject;
class Dx12Camera;
struct PrimitiveVertex;

struct SkyBoxVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT2 uv;
	//int texID;
	SkyBoxVertex() : pos{ 0,0,0,1 }, uv{ 0,0 }/*, texID(0)*/{}
	SkyBoxVertex operator=(const PrimitiveVertex& v);
};

struct SkyBoxTextures
{
	SkyBoxTextures() {}
	SkyBoxTextures(std::string front, std::string back
		, std::string right, std::string left
		, std::string top, std::string bottom)
	: front(front), back(back), right(right), left(left), top(top), bottom(bottom)
	{}

	enum TexPathID
	{
		Front,
		Back,
		Right,
		Left,
		Top,
		Bottom
	};

	/**
	*	@brief	テクスチャパスを取得する
	*	@param[in]	取得するテクスチャパスID
	*/
	std::string GetTex(TexPathID id) const;

	/**
	*	@brief	テクスチャパスを設定する
	*	@param[in]	id		設定するテクスチャのID
	*	@param[in]	path	設定するテクスチャパス
	*/
	void SetTex(TexPathID id, const std::string& path);

	std::string front;
	std::string back;
	std::string right;
	std::string left;
	std::string top;
	std::string bottom;
};

class SkyBox
{
public:
	SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList
		, const std::string skyBoxTextures[6], const std::shared_ptr<CameraHolder>& holder);
	SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList
		, const SkyBoxTextures& textures, const std::shared_ptr<CameraHolder>& holder);
	SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList
		, const std::shared_ptr<CameraHolder>& holder);
	~SkyBox();

	void Draw();

	void SetCommandList(const std::shared_ptr<Dx12CommandList>& cmdList);

	void UpdateCameraBuffer();
	
	void SetSkyBoxTextures(SkyBoxTextures& textures);
private:

	struct SkyBoxCBuffer
	{
		DirectX::XMFLOAT4X4 projections[4];
	};

	enum class Direction : int
	{
		front,
		back,
		right,
		left,
		top,
		bottom,
		max
	};

	void Init();
	DirectX::XMFLOAT4X4 GetSkyBoxProjection(std::shared_ptr<Dx12Camera> camera);
	void FixUV();
	void UpdateDescriptorHeap();
	void LoadTexture();

	std::shared_ptr<RootSignatureObject> mSkyBoxRootSignature;
	std::shared_ptr<PipelineStateObject> mSkyBoxPipelineState;
	std::vector<std::shared_ptr<TextureObject>> mTextures;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;
	std::shared_ptr<ConstantBufferObject> mProjectionBuffer;
	std::shared_ptr<Dx12CommandList> mCmdList;
	std::weak_ptr<CameraHolder> mCameraHolder;
	std::vector<unsigned int> mIndices;
	std::vector<SkyBoxVertex> mVertices;
	SkyBoxTextures mSkyBoxTextures;
	float mSkyBoxRange = 1000;
	SkyBoxCBuffer mCBufferElement;
};