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

ShaderCompiler::ShaderCompiler() : mShaderModel("5_0"), mShaderDirPath("../DirectX12Library/DirectX12/Shader/")
{
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCompiler::GetCompiledShader(const std::wstring& filePath, ID3DInclude& includer, const std::string & name, const std::string & model, unsigned int compileFlag)
{
	ID3DBlob* shader = nullptr;
	ID3DBlob* err = nullptr;
	auto result = D3DCompileFromFile(filePath.data(), mMacros.data(), &includer,
		name.data(), model.data(), compileFlag, 0, &shader, &err);
	if (err != nullptr)
	{
		OutputDebugStringA((char*)err->GetBufferPointer());
	}

	assert(SUCCEEDED(result));

	return Microsoft::WRL::ComPtr<ID3DBlob>(shader);
}

ShaderResource ShaderCompiler::GetShaderResource(int id)
{
	ShaderResource shader;
	auto resource = FindResource(nullptr, RT_RCDATA, MAKEINTRESOURCE(id));
	shader.hGlobal = LoadResource(nullptr, resource);
	shader.ptr = LockResource(shader.hGlobal);
	shader.size = SizeofResource(nullptr, resource);
	return ShaderResource();
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderCompiler::GetRootSignature(const void* ptr, size_t size)
{
	ID3DBlob* root = nullptr;
	auto result = D3DGetBlobPart(ptr, size, D3D_BLOB_ROOT_SIGNATURE, 0, &root);
	assert(SUCCEEDED(result));
	return Microsoft::WRL::ComPtr<ID3DBlob>(root);
}

ShaderCompiler::~ShaderCompiler()
{
	for (auto& data : mDatas)
	{
		for (auto& shader : data.second)
		{
			shader.second.rootSignature.Reset();
		}
	}
}

std::shared_ptr<ShaderDatas> ShaderCompiler::CompileShader(const std::string& shaderPath,
	const std::string& vsName,
	const std::string& psName,
	const std::string& gsName,
	const std::string& hsName,
	const std::string& dsName, bool existRootSignature)
{

	//auto itr = mDatas.find(shaderPath);
	//if (itr != mDatas.end())
	//{
	//	return itr->second;
	//}
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
	ID3DBlob* err = nullptr;

	std::string rpath = GetRelativePath(shaderPath);
	hlslinculde.SetRelativePath(rpath);

	std::wstring path;
	ToWChar(path, shaderPath);

	HRESULT result;

	D3D_SHADER_MACRO macro = { nullptr, nullptr };
	mMacros.push_back(macro);

	auto data = std::make_shared<ComPtrShader>();

	if (vsName.size() > 0)
	{
		data->vertex = GetCompiledShader(path, hlslinculde, vsName, "vs_" + mShaderModel, compileflag);

		if (existRootSignature)
		{
			data->rootSignature = GetRootSignature(data->vertex->GetBufferPointer(), data->vertex->GetBufferSize());
		}
	}

	if (psName.size() > 0)
	{
		data->pixel = GetCompiledShader(path, hlslinculde, psName, "ps_" + mShaderModel, compileflag);
	}

	if (gsName.size() > 0)
	{
		data->geometry = GetCompiledShader(path, hlslinculde, gsName, "gs_" + mShaderModel, compileflag);
	}

	if (hsName.size() > 0)
	{
		data->geometry = GetCompiledShader(path, hlslinculde, hsName, "hs_" + mShaderModel, compileflag);
	}

	if (dsName.size() > 0)
	{
		data->geometry = GetCompiledShader(path, hlslinculde, dsName, "ds_" + mShaderModel, compileflag);
	}

	mMacros.clear();
	mMacros.shrink_to_fit();
	mStrData.clear();
	return data;
}

std::shared_ptr<ShaderDatas> ShaderCompiler::CompileShaderFromResource(
	const int vsID,
	const int psID,
	const int gsID,
	const int hsID,
	const int dsID,
	bool existRootSignature)
{
	auto data = std::make_shared<ResourceShader>();
	if (vsID != 0)
	{
		data->vertex = GetShaderResource(vsID);
		if (existRootSignature)
		{
			data->rootSignature = GetRootSignature(data->vertex.ptr, data->vertex.size);
		}
	}

	if (psID != 0)
	{
		data->pixel = GetShaderResource(psID);
	}

	if (gsID != 0)
	{
		data->geometry = GetShaderResource(gsID);
	}

	if (hsID != 0)
	{
		data->hull = GetShaderResource(hsID);
	}

	if (dsID != 0)
	{
		data->domain = GetShaderResource(dsID);
	}

	return data;
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

void ShaderCompiler::SetShaderDirPath(const std::string& shaderDirPath)
{
	mShaderDirPath = shaderDirPath;
}

const std::string & ShaderCompiler::GetShaderDirPath() const
{
	return mShaderDirPath;
}
