#pragma once
#include <wrl.h>
#include <d3dcommon.h>

struct ShaderResource
{
	ShaderResource() : ptr(nullptr), hGlobal(nullptr), size(0) {};
	void* ptr;
	void* hGlobal;
	size_t size;
};

struct ShaderDatas
{
	Microsoft::WRL::ComPtr<ID3DBlob> rootSignature;

	virtual ShaderResource GetVS() const = 0;
	virtual ShaderResource GetPS() const = 0;
	virtual ShaderResource GetGS() const = 0;
	virtual ShaderResource GetHS() const = 0;
	virtual ShaderResource GetDS() const = 0;

	ShaderDatas();
	virtual ~ShaderDatas();
};

struct ComPtrShader : public ShaderDatas
{
public:
	ShaderResource GetVS() const;
	ShaderResource GetPS() const;
	ShaderResource GetGS() const;
	ShaderResource GetHS() const;
	ShaderResource GetDS() const;

	Microsoft::WRL::ComPtr<ID3DBlob>  vertex;
	Microsoft::WRL::ComPtr<ID3DBlob>  pixel;
	Microsoft::WRL::ComPtr<ID3DBlob>  geometry;
	Microsoft::WRL::ComPtr<ID3DBlob>  hull;
	Microsoft::WRL::ComPtr<ID3DBlob>  domain;
	Microsoft::WRL::ComPtr<ID3DBlob>  compute;
private:
	ShaderResource GetResource(const Microsoft::WRL::ComPtr<ID3DBlob>& shader) const;
};


struct ResourceShader : public ShaderDatas
{
	ShaderResource GetVS() const;
	ShaderResource GetPS() const;
	ShaderResource GetGS() const;
	ShaderResource GetHS() const;
	ShaderResource GetDS() const;

	ShaderResource vertex;
	ShaderResource pixel;
	ShaderResource geometry;
	ShaderResource hull;
	ShaderResource domain;
	ShaderResource compute;

	~ResourceShader();
};