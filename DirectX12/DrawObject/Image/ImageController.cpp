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
	, mVertex{ { { 0.f, img->GetImageSize().y, 0.f },{ 0.f, 0.f }, img->GetGamma()}/* v1 */
			,{ { img->GetImageSize().x,img->GetImageSize().y, 0.f },{ 1.f, 0.f }, img->GetGamma() }/* v2 */
			,{ { 0.0f,0.0f , 0.0f },{ 0.f, 1.f }, img->GetGamma() }/* v3 */
			,{ { img->GetImageSize().x, 0.0f, 0.f },{ 1.f, 1.f }, img->GetGamma() }/* v4 */ }
	, mScaleX(1.0f), mScaleY(1.0f), mRota(0.0f), mPivot{ 0.f,0.f,0.f }, mCenterOffset(0,0,0)
	, mRect(std::make_shared<Rect>(DirectX::XMFLOAT3(img->GetImageSize().x / 2.0f, img->GetImageSize().y / 2.0f, 0), img->GetImageSize().x, img->GetImageSize().y))
	, mTurnSign(1,1), mBundleUpdate(&ImageController::UpdateBundle)
{
	mPipelinestate = pipelinestate;
	mRootsignature = rootsignature;

	std::string name = mImgObj->GetTextureName();
	name += "2DImageVertexBuffer";
	mVertexBuffer = std::make_shared<VertexBufferObject>(name, mDevice, static_cast<unsigned int>(sizeof(ImageVertex)), 4U);

	std::vector<std::shared_ptr<Dx12BufferObject>> resource;
	resource.reserve(DEFAULT_RESOURCE_NUM);
	resource.push_back(mImgObj->GetShaderResource());

	name = mImgObj->GetTextureName();
	name += "ImageDescriptorHeap";
	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>(name, mDevice, resource, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	DirectX::XMFLOAT2 size = mImgObj->GetImageSize();
	DirectX::XMFLOAT3 offset = { size.x / 2.0f,size.y / 2.0f, 0.0f };
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMFLOAT3 vec = mVertex[i].pos - offset;
		mNormvec[i] = NormalizeXMFloat3(vec);
		mLength[i] = sqrt(DotXMFloat3(vec, vec));
	}
	SetPos(mPivot);
}

ImageController::~ImageController()
{
}

void ImageController::AddPos(const float x, const float y, const float z)
{
	mPivot.x += x;
	mPivot.y += y;
	mPivot.z += z;
	UpdateBuffer();
}

void ImageController::AddPos(const DirectX::XMFLOAT3& offset)
{
	AddPos(offset.x, offset.y, offset.z);
}

void ImageController::AddRota(const float deg)
{
	mRota += DirectX::XMConvertToRadians(deg);
	UpdateBuffer();
}

void ImageController::AddScale(const float scale)
{
	mScaleX += scale;
	mScaleY += scale;
	UpdateBuffer();
}

void ImageController::AddPivot(const DirectX::XMFLOAT3& offset)
{
	mPivot += offset;
	UpdateBuffer();
}

void ImageController::SetRect(const DirectX::XMFLOAT3& inc, const float inw, const float inh)
{
	mRect->SetCenter(inc);
	mRect->SetHeight(inh);
	mRect->SetWidth(inw);
	UpdateNormvec();
	UpdateUV();
	UpdateBuffer();
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

	UpdateBuffer();
}

void ImageController::SetPos(const DirectX::XMFLOAT3& setPos)
{
	SetPos(setPos.x, setPos.y, setPos.z);
}

void ImageController::SetScale(const float s)
{
	mScaleX = s;
	mScaleY = s;
	UpdateBuffer();
}

void ImageController::SetScale(const DirectX::XMFLOAT2 & scale)
{
	mScaleX = scale.x;
	mScaleY = scale.y;
	UpdateBuffer();
}

void ImageController::SetRota(const float deg)
{
	mRota = DirectX::XMConvertToRadians(deg);
	UpdateBuffer();
}

void ImageController::SetCenterOffset(const float x, const float y, const float z)
{
	mCenterOffset.x = x;
	mCenterOffset.y = y;
	mCenterOffset.z = z;
	UpdateNormvec();
	UpdateBuffer();
}

void ImageController::SetCenterOffset(const DirectX::XMFLOAT3& offset)
{
	SetCenterOffset(offset.x, offset.y, offset.z);
}

void ImageController::TurnU()
{
	DirectX::XMFLOAT2 uv;
	uv = mVertex[0].uv;
	mVertex[0].uv = mVertex[1].uv;
	mVertex[1].uv = uv;

	uv = mVertex[2].uv;
	mVertex[2].uv = mVertex[3].uv;
	mVertex[3].uv = uv;

	mTurnSign.x *= -1;
	UpdateNormvec();
	UpdateBuffer();
}

void ImageController::TurnV()
{
	DirectX::XMFLOAT2 uv;
	uv = mVertex[0].uv;
	mVertex[0].uv = mVertex[2].uv;
	mVertex[2].uv = uv;

	uv = mVertex[1].uv;
	mVertex[1].uv = mVertex[3].uv;
	mVertex[3].uv = uv;

	mTurnSign.y *= -1;
	UpdateNormvec();
	UpdateBuffer();
}

void ImageController::Draw()
{
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mCmdList);
	mCmdList->ExecuteBundle(mBundleCmdList);
	mCmdList->SetDrawController(shared_from_this());
}

void ImageController::BackDraw()
{
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mBackCmdList);
	mBackCmdList->ExecuteBundle(mBundleCmdList);
	mCmdList->SetDrawController(shared_from_this());
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
	rtn->SetCenterOffset(mCenterOffset);
	rtn->SetPos(mPivot);
	rtn->SetRect(mRect->GetCenter(), mRect->GetWidth(), mRect->GetHeight());
	rtn->SetRota(mRota);
	rtn->SetScale(DirectX::XMFLOAT2(mScaleX, mScaleY));
	return rtn;
}

void ImageController::SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
	mBundleUpdate = &ImageController::UpdateBundle;
}

void ImageController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mPipelinestate = pipelinestate;
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
	DirectX::XMFLOAT2 size = mImgObj->GetImageSize();
	DirectX::XMFLOAT2 leftupUV = { mRect->GetLeft() / size.x,mRect->GetDown() / size.y };
	DirectX::XMFLOAT2 rightdownUV = { mRect->GetRight() /size.x, mRect->GetUp() / size.y };
	mVertex[0].uv = leftupUV;

	mVertex[1].uv.x = rightdownUV.x;
	mVertex[1].uv.y = leftupUV.y;

	mVertex[2].uv.x = leftupUV.x;
	mVertex[2].uv.y = rightdownUV.y;

	mVertex[3].uv = rightdownUV;

	if (mTurnSign.x == -1)
	{
		DirectX::XMFLOAT2 uv;
		uv = mVertex[0].uv;
		mVertex[0].uv = mVertex[1].uv;
		mVertex[1].uv = uv;

		uv = mVertex[2].uv;
		mVertex[2].uv = mVertex[3].uv;
		mVertex[3].uv = uv;
	}
	if (mTurnSign.y == -1)
	{
		DirectX::XMFLOAT2 uv;
		uv = mVertex[0].uv;
		mVertex[0].uv = mVertex[2].uv;
		mVertex[2].uv = uv;

		uv = mVertex[1].uv;
		mVertex[1].uv = mVertex[3].uv;
		mVertex[3].uv = uv;
	}
}

void ImageController::UpdateNormvec()
{
	mVertex[0].pos.x = mRect->GetLeft() + mCenterOffset.x * mTurnSign.x;
	mVertex[0].pos.y = mRect->GetUp()	+ mCenterOffset.y * mTurnSign.y;

	mVertex[1].pos.x = mRect->GetRight() + mCenterOffset.x * mTurnSign.x;
	mVertex[1].pos.y = mRect->GetUp() + mCenterOffset.y * mTurnSign.y;

	mVertex[2].pos.x = mRect->GetLeft() + mCenterOffset.x * mTurnSign.x;
	mVertex[2].pos.y = mRect->GetDown() + mCenterOffset.y * mTurnSign.y;

	mVertex[3].pos.x = mRect->GetRight() + mCenterOffset.x * mTurnSign.x;
	mVertex[3].pos.y = mRect->GetDown() + mCenterOffset.y * mTurnSign.y;

	const DirectX::XMFLOAT3 offset = mRect->GetCenter();
	for (int i = 0; i < 4; ++i)
	{
		DirectX::XMFLOAT3 vec = mVertex[i].pos - offset;
		mNormvec[i] = NormalizeXMFloat3(vec);
		mLength[i] = sqrt(DotXMFloat3(vec, vec));
	}
}


void ImageController::UpdateBuffer()
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 size = d12.GetWindowSize();
	for (int i = 0; i < 4; ++i)
	{
		mVertex[i].pos.x = RotationXY(mNormvec[i], mRota).x * mLength[i] * mScaleX + mPivot.x;
		mVertex[i].pos.y = RotationXY(mNormvec[i], mRota).y * mLength[i] * mScaleY + mPivot.y;
		mVertex[i].pos.x *= 2.0f / size.x;
		mVertex[i].pos.y *= 2.0f / size.y;
	}
	mVertexBuffer->WriteBuffer(mVertex, sizeof(ImageVertex) * 4);
}

void ImageController::UpdateBundle()
{
	mBundleCmdList->Reset();
	auto bundle = mBundleCmdList;
	mDescHeap->SetDescriptorHeap(bundle);
	bundle->SetPipelineState(mPipelinestate);
	bundle->SetGraphicsRootSignature(mRootsignature);
	bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	bundle->IASetVertexBuffers({ &mVertexBuffer }, 1);

	mDescHeap->SetGraphicsDescriptorTable(bundle, 0, eROOT_PARAMATER_INDEX_TEXTURE);

	bundle->DrawInstanced(4, 1, 0, 0);
	bundle->Close();

	mBundleUpdate = &ImageController::NonUpdateBundle;
}

void ImageController::NonUpdateBundle()
{
}
