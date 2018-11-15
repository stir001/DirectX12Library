#include "stdafx.h"
#include "ShaderDatasStructure.h"

ShaderDatas::ShaderDatas() :rootSignature(nullptr), vertexShader(nullptr),
pixelShader(nullptr), geometryShader(nullptr), hullShader(nullptr), domainShader(nullptr)
{

}

ShaderDatas::~ShaderDatas()
{
	rootSignature.Reset();
	vertexShader.Reset();
	pixelShader.Reset();
	geometryShader.Reset();
	hullShader.Reset();
}