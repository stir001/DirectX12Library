#include "stdafx.h"
#include "PrimitiveCreator.h"
#include "PrimitivePlane.h"
#include "PrimitiveController.h"
#include "PrimitiveCube.h"
#include "PrimitiveSphere.h"
#include "PrimitiveCapsule.h"
#include "Master/Dx12Ctrl.h"
#include "d3dx12.h"
#include "Camera/Dx12Camera.h"
#include "Light/LightObject.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
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
	, mPipelineState(std::make_shared<PrimitivePipelineState>(mRootsiganture, Dx12Ctrl::Instance().GetDev()))
	, mLight(std::make_shared<DirectionalLight>(1.0f,-1.0f,1.0f))
	, mCommnadList(RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model)))
	, mNormalMapRootsignature(std::make_shared<PrimitiveNormalMapRootSignature>(Dx12Ctrl::Instance().GetDev()))
	, mNormalMapPipelineState(std::make_shared<PrimitivePipelineState>(mNormalMapRootsignature, Dx12Ctrl::Instance().GetDev()))
{
}


PrimitiveCreator::~PrimitiveCreator()
{
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreatePlane(const DirectX::XMFLOAT3& pos, float length, float height, const DirectX::XMFLOAT3& normal)
{
	return CreateController(std::shared_ptr<PrimitivePlane>(std::make_shared<PrimitivePlane>(pos, length, height, normal)));
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateCube(float length, const std::string& texPath)
{
	std::shared_ptr<PrimitiveController> rtn = CreateController(std::make_shared<PrimitiveCube>(length));
	SetTexture(rtn, texPath);
	return rtn;
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateCubeNormalMap(float length, const std::string & texPath)
{
	auto rtn = CreateCube(length, texPath);
	rtn->SetGraphicsRootSignature(mNormalMapRootsignature);
	rtn->SetPipelineState(mNormalMapPipelineState);
	return rtn;
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateSphere(float radius, unsigned int div, const std::string& texPath)
{
	auto sph = std::make_shared<PrimitiveSphere>(radius, div);
	auto rtn = CreateController(sph);
	SetTexture(rtn, texPath);
	return rtn;
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateCapsule(float radius, float length, const std::string & texPath)
{
	auto cap = std::make_shared<PrimitiveCapsule>(radius, length);
	auto rtn = CreateController(cap);
	return rtn;
}

void PrimitiveCreator::SetParamaters(std::shared_ptr<PrimitiveController>& ctrl)
{
	ctrl->SetGraphicsRootSignature(mRootsiganture);
	ctrl->SetPipelineState(mPipelineState);
	ctrl->SetLightBuffer(mLight);
}

std::shared_ptr<PrimitiveController> PrimitiveCreator::CreateController(const std::shared_ptr<PrimitiveObject>& primitive)
{
	std::shared_ptr<PrimitiveController> rtn = std::make_shared<PrimitiveController>(primitive, Dx12Ctrl::Instance().GetDev(), mCommnadList);
	SetParamaters(rtn);
	return rtn;
}

void PrimitiveCreator::SetTexture(std::shared_ptr<PrimitiveController>& ctrl, const std::string & texPath)
{
	if (texPath != "")
	{
		ctrl->SetTexture(TextureLoader::Instance().LoadTexture(texPath));
	}
	else
	{
		
	}
}

void PrimitiveCreator::SetLightObject(std::shared_ptr<LightObject> inlight)
{
	mLight = inlight;
}

void PrimitiveCreator::SetRenderingCommandList(std::shared_ptr<Dx12CommandList>& cmdList)
{
	mCommnadList = cmdList;
}
