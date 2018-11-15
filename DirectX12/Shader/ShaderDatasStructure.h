#pragma once
#include <wrl.h>
#include <d3dcommon.h>

struct ShaderDatas
{
	Microsoft::WRL::ComPtr<ID3DBlob> rootSignature;
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> geometryShader;
	Microsoft::WRL::ComPtr<ID3DBlob> hullShader;
	Microsoft::WRL::ComPtr<ID3DBlob> domainShader;
	Microsoft::WRL::ComPtr<ID3DBlob> computeShader;
	ShaderDatas();
	~ShaderDatas();
};