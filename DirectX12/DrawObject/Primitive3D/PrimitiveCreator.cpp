#include "stdafx.h"
#include "PrimitiveCreator.h"
#include "PrimitivePlane.h"
#include "PrimitiveController.h"
#include "PrimitiveCube.h"
#include "PrimitiveSphere.h"
#include "Master/Dx12Ctrl.h"
#include "d3dx12.h"
#include "Camera/Dx12Camera.h"
#include "Light/LightObject.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "Rootsignature/PrimitiveRootSignature.h"
#include "Rootsignature/PrimitiveNormalMapRootSignature.h"
#include "PipelineState/PrimitivePipelineState.h"
#include "Light/DirectionalLight.h"
#include "Texture/TextureLoader.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <tchar.h>

PrimitiveCreator* PrimitiveCreator::mInstance = nullptr;

PrimitiveCreator::PrimitiveCreator():mRootsiganture(std::make_shared<PrimitiveRootSignature>(Dx12Ctrl::Instance().GetDev()))
	,mPipelineState(std::make_shared<PrimitivePipelineState>(mRootsiganture, Dx12Ctrl::Instance().GetDev()))
	,mLight(std::make_shared<DirectionalLight>(1.0f,-1.0f,0.0f)),mCommnadList(RenderingPathManager::Instance().GetRenderingPathCommandList(0))
	,mNormalMapRootsignature(std::make_shared<PrimitiveNormalMapRootSignature>(Dx12Ctrl::Instance().GetDev()))
	,mNormalMapPipelineState(std::make_shared<PrimitivePipelineState>(mNormalMapRootsignature, Dx12Ctrl::Instance().GetDev()))
{
}


PrimitiveCreator::~PrimitiveCreator()
{
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreatePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 normal)
{
	return CreateController(std::shared_ptr<PrimitivePlane>(std::make_shared<PrimitivePlane>(pos, length, height, normal)));
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateCube(float length, const std::string& texPath)
{
	auto tex = TextureLoader::Instance().LoadTexture(texPath);
	std::shared_ptr<PrimitiveController> rtn = CreateController(std::make_shared<PrimitiveCube>(length));
	rtn->SetTexture(tex);
	return rtn;
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateCubeNormalMap(float length, const std::string & texPath)
{
	auto rtn = CreateCube(length, texPath);
	rtn->SetRootSignature(mNormalMapRootsignature);
	rtn->SetPipelineState(mNormalMapPipelineState);
	return rtn;
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateSphere(float radius, unsigned int div, const std::string& texPath)
{
	auto tex = TextureLoader::Instance().LoadTexture(texPath);
	auto sph = std::make_shared<PrimitiveSphere>(radius, div);
	auto rtn = CreateController(sph);
	rtn->SetTexture(tex);
	return rtn;
}

void PrimitiveCreator::SetParamaters(std::shared_ptr<PrimitiveController>& ctrl)
{
	ctrl->SetRootSignature(mRootsiganture);
	ctrl->SetPipelineState(mPipelineState);
	ctrl->SetLightBuffer(mLight);
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateController(const std::shared_ptr<PrimitiveObject>& primitive)
{
	std::shared_ptr<PrimitiveController> rtn = std::make_shared<PrimitiveController>(primitive, Dx12Ctrl::Instance().GetDev(), mCommnadList);
	SetParamaters(rtn);
	return rtn;
}

void PrimitiveCreator::SetLightObject(std::shared_ptr<LightObject> inlight)
{
	mLight = inlight;
}
