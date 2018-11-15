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
#include "PipelineState/Image2DPipelineState.h"
#include "PipelineState//Image3DPipelineState.h"
#include "PipelineState/BillboardPipelineState.h"
#include "Shader/ShaderCompiler.h"
#include "Texture/TextureLoader.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "DrawObject/Image/Image3DController.h"

#include "d3dx12.h"

ImageLoader* ImageLoader::mInstance = nullptr;

ImageLoader::ImageLoader()
{
	DX12CTRL_INSTANCE;
	CreateRootsignature(d12.GetDev());
	CreatePipelineState(d12.GetDev());
	mCmdList = RenderingPathManager::Instance().GetRenderingPathCommandList(0);
}

ImageLoader::~ImageLoader()
{
	mImages.clear();
	mRootsignature.reset();
	mPipelinestate.reset();
	m3DPipelinestate.reset();
	m3DPipelinestate.reset();
}

std::shared_ptr<ImageController> ImageLoader::LoadImageData(const std::string& path)
{
	std::shared_ptr<ImageController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		imgCtrl = std::make_shared<ImageController>(itr->second, Dx12Ctrl::Instance().GetDev(), mCmdList, mPipelinestate, mRootsignature);
		return imgCtrl;
	}
	
	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance().LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj = ImageObject::Create(tObj->GetWidth(), tObj->GetHeight(), tObj);
	mImages[path] = imgObj;
	imgCtrl = std::make_shared<ImageController>(imgObj, Dx12Ctrl::Instance().GetDev(), mCmdList,mPipelinestate,mRootsignature);

	return imgCtrl;
}

std::shared_ptr<Image3DController> ImageLoader::LoadImage3D(const std::string& path, bool isBillboard)
{
	std::shared_ptr<Image3DController> imgCtrl;
	auto itr = mImages.find(path);
	if (itr != mImages.end())
	{
		imgCtrl = std::make_shared<Image3DController>(itr->second, Dx12Ctrl::Instance().GetDev(), mCmdList, m3DPipelinestate, m3DRootsignature);
		return imgCtrl;
	}

	std::shared_ptr<TextureObject> tObj = TextureLoader::Instance().LoadTexture(path);
	std::shared_ptr<ImageObject> imgObj = ImageObject::Create(tObj->GetWidth(), tObj->GetHeight(), tObj);
	mImages[path] = imgObj;
	if(isBillboard)
	{ 
		imgCtrl = std::make_shared<Image3DController>(imgObj, Dx12Ctrl::Instance().GetDev(), mCmdList, mBillboardPipelineState, mBillboardRootsignature );
	}
	else
	{
		imgCtrl = std::make_shared<Image3DController>(imgObj, Dx12Ctrl::Instance().GetDev(), mCmdList, m3DPipelinestate, m3DRootsignature);
	}
	return imgCtrl;
}

void ImageLoader::Release(const std::string& releaseImagePath)
{
	TextureLoader::Instance().Release(releaseImagePath);
	mImages.erase(releaseImagePath);
}

void ImageLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mPipelinestate = std::make_shared <Image2DPipelineState>(mRootsignature, dev);

	m3DPipelinestate = std::make_shared<Image3DPipelineState>(m3DRootsignature, dev);

	mBillboardPipelineState = std::make_shared<BillboardPipelineState>(mBillboardRootsignature, dev);
}

void ImageLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mRootsignature = std::make_shared<Image2DRootSignature>(dev);
	
	m3DRootsignature = std::make_shared<Image3DRootSignature>(dev);

	mBillboardRootsignature = std::make_shared<BillboardRootSignature>(dev);
}
