#include "ShaderCompiler.h"
#include "stdafx.h"
#include "ShaderCompiler.h"
#include "Util/Util.h"
#include "Util/CharToWChar.h"
#include "Shader/HlslInclude.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <functional>
#include <d3dcommon.h>
#include <cassert>

using Microsoft::WRL::ComPtr;

ShaderCompiler* ShaderCompiler::mInstance = nullptr;

ShaderCompiler::ShaderCompiler() : mShaderModel("5_0"), mShaderDirPath("Dx12MSLib/DirectX12/Shader/")
{
}

ShaderCompiler::~ShaderCompiler()
{
	for (auto& data : mDatas)
	{
		data.second.vertexShader.Reset();
		data.second.pixelShader.Reset();
		data.second.geometryShader.Reset();
		data.second.domainShader.Reset();
		data.second.hullShader.Reset();
		data.second.rootSignature.Reset();
	}
}

ShaderDatas ShaderCompiler::CompileShader(const std::string& shaderPath,
	const std::string& vsName,
	const std::string& psName,
	const std::string& gsName,
	const std::string& hsName,
	const std::string& dsName, bool existRootSignature)
{

	auto itr = mDatas.find(shaderPath);
	if (itr != mDatas.end())
	{
		return itr->second;
	}
#ifdef _DEBUG
	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
		if (err != nullptr)
		{
			OutputDebugStringA((char*)err->GetBufferPointer());
		}
	};
#else
	std::function<void(ID3D10Blob*)> outErr = [](ID3D10Blob* err) {
	};
	UINT compileflag = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

	HlslInclude hlslinculde;
	ID3D10Blob* err = nullptr;

	std::string rpath = GetRelativePath(shaderPath);
	hlslinculde.SetRelativePath(rpath);

	wchar_t* path = nullptr;
	ToWChar(&path, shaderPath);

	HRESULT result;

	D3D_SHADER_MACRO macro = { nullptr, nullptr };
	mMacros.push_back(macro);

	ID3D12ShaderReflection* id3d12ref = nullptr;

	if (vsName.size() > 0)
	{
		ID3DBlob* vertex = nullptr;
		std::string vsModel = "vs_" + mShaderModel;
		result = D3DCompileFromFile(path, mMacros.data(), &hlslinculde,
			vsName.data(), vsModel.data(), compileflag, 0, &vertex, &err);
		outErr(err);
		mDatas[shaderPath].vertexShader.Swap(vertex);

		assert(SUCCEEDED(result));

		if (existRootSignature)
		{
			ID3DBlob* root = nullptr;
			result = D3DGetBlobPart(vertex->GetBufferPointer() , vertex->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &root);
			mDatas[shaderPath].rootSignature.Swap(root);
			assert(SUCCEEDED(result));
		}
	}

	if (psName.size() > 0)
	{
		ID3DBlob* pixcel = nullptr;
		std::string psModel = "ps_" + mShaderModel;
		result = D3DCompileFromFile(path, mMacros.data(), &hlslinculde,
			psName.data(), psModel.data(), compileflag, 0, &pixcel, &err);
		outErr(err);
		mDatas[shaderPath].pixelShader.Swap(pixcel);
		assert(SUCCEEDED(result));
	}

	if (gsName.size() > 0)
	{
		ID3DBlob* geometry = nullptr;
		std::string gsModel = "gs_" + mShaderModel;
		result = D3DCompileFromFile(path, mMacros.data(), &hlslinculde,
			gsName.data(), gsModel.data(), compileflag, 0, &geometry, &err);
		outErr(err);
		mDatas[shaderPath].geometryShader.Swap(geometry);
		assert(SUCCEEDED(result));
	}

	if (hsName.size() > 0)
	{
		ID3DBlob* hull = nullptr;
		std::string hsModel = "hs_" + mShaderModel;
		result = D3DCompileFromFile(path, mMacros.data(), &hlslinculde,
			hsName.data(), hsModel.data(), compileflag, 0, &hull, &err);
		outErr(err);
		mDatas[shaderPath].hullShader.Swap(hull);
		assert(SUCCEEDED(result));
	}

	if (dsName.size() > 0)
	{
		ID3DBlob* domain = nullptr;
		std::string dsModel = "ds_" + mShaderModel;
		result = D3DCompileFromFile(path, mMacros.data(), &hlslinculde,
			dsName.data(), dsModel.data(), compileflag, 0, &domain, &err);
		outErr(err);
		mDatas[shaderPath].domainShader.Swap(domain);
		assert(SUCCEEDED(result));
	}

	mMacros.clear();
	mMacros.shrink_to_fit();
	mStrData.clear();
	return mDatas[shaderPath];
}

void ShaderCompiler::ReleaseShader(std::string shaderpath)
{
	mDatas.erase(shaderpath);
}

void ShaderCompiler::AddDefineMacro(const std::string & name, const std::string & def)
{
	MacroData strData;
	strData.name = name;
	strData.def = def;
	mStrData.push_back(strData);
	D3D_SHADER_MACRO macro;
	macro.Name = mStrData.back().name.data() + '\0';
	macro.Definition = mStrData.back().def.data() + '\0';
	mMacros.push_back(macro);
}

void ShaderCompiler::SetShaderModel(const std::string& shaderModel)
{
	mShaderModel = shaderModel;
}

void ShaderCompiler::SetShaderDirPath(std::string shaderDirPath)
{
	mShaderDirPath = shaderDirPath;
}

const std::string & ShaderCompiler::GetShaderDirPath() const
{
	return mShaderDirPath;
}
