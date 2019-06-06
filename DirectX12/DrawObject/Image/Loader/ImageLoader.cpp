#include "stdafx.h"
#include "ImageLoader.h"
#include "DrawObject/Image/ImageObject.h"
#include "Master/Dx12Ctrl.h"
#include "Util/CharToWChar.h"
#include "Texture/TextureObject.h"
#include "DrawObject/Image/ImageController.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Rootsignature/Image2DRootSignature.h"
#include "Rootsignature/Image3DRootSignature.h"
#include "Rootsignature/BillboardRootSignature.h"
#include "PipelineState/PipelineStateObject.h"
#include "PipelineState/NotCullPipelineState.h"
#include "PipelineState//Image3DPipelineState.h"
#include "PipelineState/DefaultPipelineState.h"
#include "Shader/ShaderCompiler.h"
#include "Texture/TextureLoader.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "DrawObject/Image/Image3DController.h"

ImageLoader* ImageLoader::mInstance = nullptr;

ImageLoader::ImageLoader()
{
	DX12CTRL_INSTANCE;
	CreateRootsignature(d12.GetDev());
	CreatePipelineState(d12.GetDev());
	mCmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::UI));
	mBackCmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::BackGround));
	mModelCmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model));
}

ImageLoader::~ImageLoader()
{
	mImages.clear();
}

std::shared_ptr<ImageController> ImageLoader::LoadImage2D(const std::string& path)
{
	std::shared_ptr<ImageController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		imgCtrl = std::make_shared<ImageController>(itr->second, Dx12Ctrl::Instance().GetDev(), mCmdList, mBackCmdList, mPipelinestate, mRootsignature);
		return imgCtrl;
	}
	
	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance().LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj = ImageObject::Create(tObj->GetWidth(), tObj->GetHeight(), tObj);
	mImages[path] = imgObj;
	imgCtrl = std::make_shared<ImageController>(imgObj, Dx12Ctrl::Instance().GetDev(), mCmdList, mBackCmdList, mPipelinestate,mRootsignature);

	return imgCtrl;
}

std::shared_ptr<Image3DController> ImageLoader::LoadImage3D(const std::string& path, bool isBillboard)
{
	std::shared_ptr<Image3DController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		if(isBillboard)
		{ 
			imgCtrl = std::make_shared<Image3DController>(itr->second, Dx12Ctrl::Instance().GetDev(), mModelCmdList, mBillboardPipelineState, mBillboardRootsignature);
		}
		else
		{
			imgCtrl = std::make_shared<Image3DController>(itr->second, Dx12Ctrl::Instance().GetDev(), mModelCmdList, m3DPipelinestate, m3DRootsignature);
		}
		return imgCtrl;
	}

	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance().LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj = ImageObject::Create(tObj->GetWidth(), tObj->GetHeight(), tObj);
	mImages[path] = imgObj;
	if(isBillboard)
	{ 
		imgCtrl = std::make_shared<Image3DController>(imgObj, Dx12Ctrl::Instance().GetDev(), mModelCmdList, mBillboardPipelineState, mBillboardRootsignature );
	}
	else
	{
		imgCtrl = std::make_shared<Image3DController>(imgObj, Dx12Ctrl::Instance().GetDev(), mModelCmdList, m3DPipelinestate, m3DRootsignature);
	}
	return imgCtrl;
}

void ImageLoader::SetUIPassCommandList(std::shared_ptr<Dx12CommandList>& cmdList)
{
	mCmdList = cmdList;
}

void ImageLoader::SetBackGroundPassCommandList(std::shared_ptr<Dx12CommandList>& cmdList)
{
	mBackCmdList = cmdList;
}

void ImageLoader::Set3DPassCommandList(std::shared_ptr<Dx12CommandList>& cmdList)
{
	mModelCmdList = cmdList;
}

void ImageLoader::Release(const std::string& releaseImagePath)
{
	TextureLoader::Instance().Release(releaseImagePath);
	mImages.erase(releaseImagePath);
}

void ImageLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mPipelinestate = std::make_shared <NotCullPipelineState>("Image2D", mRootsignature, dev);

	m3DPipelinestate = std::make_shared<NotCullPipelineState>("Image3D", m3DRootsignature, dev);

	mBillboardPipelineState = std::make_shared<NotCullPipelineState>("Billboard",mBillboardRootsignature, dev);
}

void ImageLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mRootsignature = std::make_shared<Image2DRootSignature>(dev);
	
	m3DRootsignature = std::make_shared<Image3DRootSignature>(dev);

	mBillboardRootsignature = std::make_shared<BillboardRootSignature>(dev);
}
