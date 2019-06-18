#include "stdafx.h"
#include "ShaderDatasStructure.h"

ShaderDatas::ShaderDatas() :rootSignature(nullptr)
{

}

ShaderDatas::~ShaderDatas()
{
	rootSignature.Reset();
}

ShaderResource ComPtrShader::GetVS() const
{
	return GetResource(vertex);
}

ShaderResource ComPtrShader::GetPS() const
{
	return GetResource(pixel);
}

ShaderResource ComPtrShader::GetGS() const
{
	return GetResource(geometry);
}

ShaderResource ComPtrShader::GetHS() const
{
	return GetResource(hull);
}

ShaderResource ComPtrShader::GetDS() const
{
	return GetResource(domain);
}

ShaderResource ComPtrShader::GetResource(const Microsoft::WRL::ComPtr<ID3DBlob>& shader) const
{
	return { shader->GetBufferPointer(), shader->GetBufferSize()};
}

ShaderResource ResourceShader::GetVS() const
{
	return vertex;
}

ShaderResource ResourceShader::GetPS() const
{
	return pixel;
}

ShaderResource ResourceShader::GetGS() const
{
	return geometry;
}

ShaderResource ResourceShader::GetHS() const
{
	return hull;
}

ShaderResource ResourceShader::GetDS() const
{
	return domain;
}

ResourceShader::~ResourceShader()
{
}
