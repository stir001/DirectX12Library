#include "stdafx.h"
#include "ImageController.h"
#include "Master/Dx12Ctrl.h"
#include "Util/XMFloatOperators.h"
#include "DrawObject/Image/ImageObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "Util/Rect.h"
#include "PipelineState/PipelineStateObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "CommandList/Dx12CommandList.h"
#include "ImageDrawIssuer.h"

DirectX::XMFLOAT3 RotationXY(DirectX::XMFLOAT3& pos, float rad)
{
	DirectX::XMFLOAT3 rtn = {};
	rtn.x = pos.x * cos(rad) - pos.y * sin(rad);
	rtn.y = pos.x * sin(rad) + pos.y * cos(rad);
	return rtn;
}

const unsigned int DEFAULT_RESOURCE_NUM = 1;

ImageController::ImageController(std::shared_ptr<ImageObject> img,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	std::shared_ptr<Dx12CommandList> uicmdList,
	std::shared_ptr<Dx12CommandList> backcmdList,
	std::shared_ptr<PipelineStateObject>& pipelinestate,
	std::shared_ptr<RootSignatureObject>& rootsignature)
	:DrawObjectController(img->GetTextureName() + "Bundle", dev, uicmdList)
	, mBackCmdList(backcmdList)
	, mImgObj(img)
	, mVertex{ { { -img->GetImageSize().x / 2.0f, img->GetImageSize().y / 2.0f, 0.0f, 1.0f }, img->GetGamma(), 0U}/* v1 */
			,{ { img->GetImageSize().x / 2.0f, img->GetImageSize().y / 2.0f, 0.0f, 1.0f }, img->GetGamma(), 1U}/* v2 */
			,{ { -img->GetImageSize().x / 2.0f, -img->GetImageSize().y / 2.0f, 0.0f, 1.0f }, img->GetGamma(), 2U }/* v3 */
			,{ { img->GetImageSize().x / 2.0f, -img->GetImageSize().y / 2.0f, 0.0f, 1.0f }, img->GetGamma(), 3U }/* v4 */ }
	, mScale(1.0f, 1.0f), mRota(0.0f), mPivot{ 0.0f,0.0f,0.0f }, mCenterOffset(0.0f, 0.0f, 0.0f)
	, mRect(std::make_shared<Rect>(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), img->GetImageSize().x, img->GetImageSize().y))
	, mTurnSign(1,1), mBundleUpdate(&ImageController::UpdateBundle)
{
	mPipelinestate = pipelinestate;
	mRootsignature = rootsignature;

	std::string name = mImgObj->GetTextureName();
	name += "2DImageVertexBuffer";
	mVertexBuffer = std::make_shared<VertexBufferObject>(name, mDevice, static_cast<unsigned int>(sizeof(mVertex[0])), 4U);
	mVertexBuffer->WriteBuffer(mVertex, sizeof(mVertex[0]) * 4U);
	name = mImgObj->GetTextureName();
	name += "2DImageUVBuffer";
	mUVBuffer = std::make_shared<VertexBufferObject>(name, mDevice, static_cast<unsigned int>(sizeof(mUVs[0])), 1U);
	name = mImgObj->GetTextureName();
	name += "2DImageMatrixBuffer";
	mMatrixBuffer = std::make_shared<VertexBufferObject>(name, mDevice, static_cast<unsigned int>(sizeof(mInstanceMat)), 1U);

	std::vector<std::shared_ptr<Dx12BufferObject>> resource;
	resource.reserve(DEFAULT_RESOURCE_NUM);
	resource.push_back(mImgObj->GetShaderResource());

	name = mImgObj->GetTextureName();
	name += "ImageDescriptorHeap";
	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>(name, mDevice, resource, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CreateDrawIssuer();
	CreateInstanceData();

	SetPos(mPivot);
	UpdateInstanceMatrix();
	UpdateUV();
}

ImageController::~ImageController()
{
}

void ImageController::AddPos(const float x, const float y, const float z)
{
	mPivot.x += x;
	mPivot.y += y;
	mPivot.z += z;
	UpdateInstanceMatrix();
}

void ImageController::AddPos(const DirectX::XMFLOAT3& offset)
{
	AddPos(offset.x, offset.y, offset.z);
}

void ImageController::AddRota(const float rad)
{
	mRota += rad;
	UpdateInstanceMatrix();
}

void ImageController::AddScale(const float scale)
{
	mScale.x += scale;
	mScale.y += scale;
	UpdateInstanceMatrix();
}

void ImageController::AddPivot(const DirectX::XMFLOAT3& offset)
{
	mPivot += offset;
	UpdateInstanceMatrix();
}

void ImageController::SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh)
{
	mRect->SetCenter(inc);
	mRect->SetHeight(inh);
	mRect->SetWidth(inw);
	UpdateUV();
}

void ImageController::SetRect(const Rect& rc)
{
	SetRect(rc.GetCenter(), rc.GetWidth(), rc.GetHeight());
}

void ImageController::SetPos(const float x, const float y, const float z)
{
	mPivot.x = x;
	mPivot.y = y;
	mPivot.z = z;

	UpdateInstanceMatrix();
}

void ImageController::SetPos(const DirectX::XMFLOAT3& setPos)
{
	SetPos(setPos.x, setPos.y, setPos.z);
}

void ImageController::SetScale(const float s)
{
	mScale.x = s;
	mScale.y = s;
	UpdateInstanceMatrix();
}

void ImageController::SetScale(const DirectX::XMFLOAT2 & scale)
{
	mScale.x = scale.x;
	mScale.y = scale.y;
	UpdateInstanceMatrix();
}

void ImageController::SetRota(const float rad)
{
	mRota = rad;
	UpdateInstanceMatrix();
}

void ImageController::SetCenterOffset(const float x, const float y, const float z)
{
	mCenterOffset.x = x;
	mCenterOffset.y = y;
	mCenterOffset.z = z;
	UpdateInstanceMatrix();
}

void ImageController::SetCenterOffset(const DirectX::XMFLOAT3& offset)
{
	SetCenterOffset(offset.x, offset.y, offset.z);
}

void ImageController::TurnU()
{
	unsigned int instanceID = GetCurrentInstanceID();
	std::swap(mUVs[instanceID].uv[0], mUVs[instanceID].uv[1]);
	std::swap(mUVs[instanceID].uv[2], mUVs[instanceID].uv[3]);

	mTurnSign.x *= -1;
	WriteUVBuffer();
}

void ImageController::TurnV()
{
	unsigned int instanceID = GetCurrentInstanceID();
	std::swap(mUVs[instanceID].uv[0], mUVs[instanceID].uv[2]);
	std::swap(mUVs[instanceID].uv[1], mUVs[instanceID].uv[3]);

	mTurnSign.y *= -1;
	WriteUVBuffer();
}

void ImageController::Draw()
{
	(this->*mBundleUpdate)();
	CreateInstanceData();
	if (mDrawIssuer->CountUpInstanceNum() == 1)
	{
		mCmdList->SetDrawCallIssuer(mDrawIssuer);
		mCmdList->SetDrawController(shared_from_this());
	}
}

void ImageController::BackDraw()
{
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mBackCmdList);
	mBackCmdList->ExecuteBundle(mBundleCmdList);
	mBackCmdList->SetDrawController(shared_from_this());
}

bool ImageController::IsTurnU() const
{
	return mTurnSign.x == -1;
}

bool ImageController::IsTurnV() const
{
	return mTurnSign.y == -1;
}

DirectX::XMFLOAT2 ImageController::GetImageSize()
{
	return mImgObj->GetImageSize();
}

std::shared_ptr<ImageController> ImageController::Duplicate()
{
	std::shared_ptr<ImageController> rtn  = std::make_shared<ImageController>(mImgObj, mDevice, mCmdList, mBackCmdList, mPipelinestate, mRootsignature);
	rtn->SetRect(mRect->GetCenter(), mRect->GetWidth(), mRect->GetHeight());
	rtn->SetCenterOffset(mCenterOffset);
	rtn->SetPos(mPivot);
	rtn->SetRota(mRota);
	rtn->SetScale(mScale);
	return rtn;
}

void ImageController::SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mDrawIssuer->SetRootSignature(rootsignature);
	mBundleUpdate = &ImageController::UpdateBundle;
}

void ImageController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mDrawIssuer->SetPipelineState(pipelinestate);
	mBundleUpdate = &ImageController::UpdateBundle;
}

std::string ImageController::GetImageName() const
{
	return mImgObj->GetTextureName();
}

std::string ImageController::GetFilePath() const
{
	return mImgObj->GetFilePath();
}

void ImageController::UpdateUV()
{
	CreateInstanceData();

	DirectX::XMFLOAT2 size = mImgObj->GetImageSize();
	DirectX::XMFLOAT2 leftupUV = { (size.x / 2.0f + mRect->GetLeft()) / size.x, 1.0f - (size.y / 2.0f + mRect->GetUp()) / size.y };
	DirectX::XMFLOAT2 rightdownUV = { (size.x / 2.0f + mRect->GetRight()) /size.x, 1.0f - (size.y / 2.0f + mRect->GetDown()) / size.y };
	mCalUV.uv[0] = leftupUV;

	mCalUV.uv[1].x = rightdownUV.x;
	mCalUV.uv[1].y = leftupUV.y;

	mCalUV.uv[2].x = leftupUV.x;
	mCalUV.uv[2].y = rightdownUV.y;

	mCalUV.uv[3] = rightdownUV;

	if (mTurnSign.x == -1)
	{
		std::swap(mCalUV.uv[0], mCalUV.uv[1]);
		std::swap(mCalUV.uv[2], mCalUV.uv[3]);
	}
	if (mTurnSign.y == -1)
	{
		std::swap(mCalUV.uv[0], mCalUV.uv[2]);
		std::swap(mCalUV.uv[1], mCalUV.uv[3]);
	}

	mUVs[GetCurrentInstanceID()] = mCalUV;

	WriteUVBuffer();
}

void ImageController::UpdateInstanceMatrix()
{
	CreateInstanceData();

	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12.GetWindowSize();
	auto mat = IdentityXMFloat4x4();
	mCalMatrix = mat;
	mat._11 = 2.0f / size.x;
	mat._22 = 2.0f / size.y;
	
	mCalMatrix *= ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixScaling(mScale.x, mScale.y, 1.0f));
	auto offsetMat = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixTranslation(mCenterOffset.x, mCenterOffset.y, mCenterOffset.z));
	mCalMatrix *= offsetMat;
	mCalMatrix *= ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixRotationZ(mRota));
	mCalMatrix *= -offsetMat;
	mCalMatrix *= ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixTranslation(mPivot.x ,mPivot.y, mPivot.z));
	mCalMatrix *= mat;

	mInstanceMat[GetCurrentInstanceID()] = mCalMatrix;

	WriteMatrixBuffer();
}

void ImageController::UpdateBundle()
{
	mDrawIssuer->UpdateBundle();

	mBundleUpdate = &ImageController::NonUpdateBundle;
}

void ImageController::NonUpdateBundle()
{
}

void ImageController::CreateDrawIssuer()
{
	ImageDrawIssuer::ConstructorArg arg = {};
	arg.descHeap = mDescHeap;
	arg.uvBuffer = mUVBuffer;
	arg.vertexBuffer = mVertexBuffer;
	arg.matrixBuffer = mMatrixBuffer;
	arg.name = mImgObj->GetTextureName();
	arg.pipelineState = mPipelinestate;
	arg.rootSignature = mRootsignature;
	arg.texViewIndex = 0;
	arg.dev = mDevice;
	
	mDrawIssuer = std::make_shared<ImageDrawIssuer>(arg);
}

void ImageController::WriteMatrixBuffer()
{
	mMatrixBuffer->WriteBuffer(mInstanceMat.data(), static_cast<unsigned int>(sizeof(mInstanceMat[0]) * mInstanceMat.size()));
}

void ImageController::WriteUVBuffer()
{
	mUVBuffer->WriteBuffer(mUVs.data(), static_cast<unsigned int>(sizeof(mUVs[0]) * mUVs.size()));
}

void ImageController::CreateInstanceData()
{
	unsigned int instanceNum = GetCurrentInstanceID() + 1;
	if (instanceNum > mUVs.size())
	{
		mUVs.resize(instanceNum);
		mUVs[GetCurrentInstanceID()] = mCalUV;
		RemakeVertexBuffer(mUVBuffer, static_cast<unsigned int>(sizeof(mUVs[0])), instanceNum);
		mDrawIssuer->SetUVBuffer(mUVBuffer);
		WriteUVBuffer();
	}
	if (instanceNum > mInstanceMat.size())
	{
		mInstanceMat.resize(instanceNum);
		mInstanceMat[GetCurrentInstanceID()] = mCalMatrix;
		RemakeVertexBuffer(mMatrixBuffer, static_cast<unsigned int>(sizeof(mInstanceMat[0])), instanceNum);
		mDrawIssuer->SetMatrixBuffer(mMatrixBuffer);
		WriteMatrixBuffer();
	}
}

unsigned int ImageController::GetCurrentInstanceID() const
{
	return mDrawIssuer->GetInstanceNum();
}

void ImageController::RemakeVertexBuffer(std::shared_ptr<VertexBufferObject>& buffer, unsigned int elementSize, unsigned int elementNum)
{
	auto name = buffer->GetName();
	buffer = std::make_shared<VertexBufferObject>(name, mDevice, elementSize, elementNum);
}
