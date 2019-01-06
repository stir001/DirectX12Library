#include "stdafx.h"
#include "CameraHolder.h"
#include "Camera/Dx12Camera.h"
#include "DrawObject/DrawController3D.h"
#include "Buffer/ConstantBufferObject.h"

CameraHolder::CameraHolder(int wndWidth, int wndHeight, Microsoft::WRL::ComPtr<ID3D12Device> dev)
	:mWndSize{wndWidth, wndHeight},
	mDevice(dev),
	mCamerasBuffer(std::make_shared<ConstantBufferObject>("MultiCameraConstntBuffer", dev, static_cast<unsigned int>(sizeof(MultiCameras)), 1U))
{
	UpdateBuffers();
}

CameraHolder::~CameraHolder()
{
}

std::shared_ptr<Dx12Camera> CameraHolder::CreateCamera(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, 
	D3D12_VIEWPORT viewport, D3D12_RECT scissorRect)
{
	std::shared_ptr<Dx12Camera> rtn = nullptr;
	if (mCameras.size() < MAXCAMERA_NUM)
	{
		rtn = std::make_shared<Dx12Camera>(viewport, scissorRect, eye
			, target, DirectX::XMFLOAT3(0.f, 1.f, 0.f)
			, shared_from_this(), static_cast<unsigned int>(mCameras.size()));
		mCameras.push_back(rtn);
		mMultiCameras.cameraBuffer[mCameras.size() - 1] = rtn->GetCameraBufferElement();
		mMultiCameras.cameraNum = static_cast<unsigned int>(mCameras.size());
		mViewPorts.push_back(rtn->GetViewPort());
		mScissorRects.push_back(rtn->GetScissorRect());
		UpdateBuffers();
	}
	return rtn;
}

std::shared_ptr<Dx12Camera> CameraHolder::GetCamera(unsigned int index) const
{
	if (index >= static_cast<unsigned int>(mCameras.size())) return nullptr;
	return mCameras[index];
}

int CameraHolder::GetCameraNum() const
{
	return mCameras.size();
}

bool CameraHolder::DeleteCamera(unsigned int index)
{
	bool rtn = false;
	if (index >= mCameras.size()) return rtn;
	auto itr = mCameras.begin() + index;
	auto rtnItr = mCameras.erase(itr);
	if (rtnItr == mCameras.end())
	{
		mCameras.shrink_to_fit();
		mViewPorts.erase(mViewPorts.begin() + index);
		mScissorRects.erase(mScissorRects.begin() + index);
		mMultiCameras.cameraNum = static_cast<unsigned int>(mCameras.size());
		UpdateBuffers();
		rtn = true;
	}
	return rtn;
}

void CameraHolder::SetCameraBuffer(DrawController3D* controller)
{
	controller->SetCameraBuffer(mCamerasBuffer);
	mControllers.push_back(controller);
}

void CameraHolder::SetCameraElement(Dx12Camera* camera)
{
	int index = camera->GetHoldIndex();
	if (index < 0 || index >= MAXCAMERA_NUM) return;
	mMultiCameras.cameraBuffer[index] = camera->GetCameraBufferElement();
	UpdateBuffers();
}

void CameraHolder::SetCameraViewPort(Dx12Camera * camera)
{
	int index = camera->GetHoldIndex();
	if (index > mViewPorts.size()) return;
	mViewPorts[index] = camera->GetViewPort();
}

void CameraHolder::SetCameraScissorRect(Dx12Camera * camera)
{
	int index = camera->GetHoldIndex();
	if (index > mScissorRects.size()) return;
	mScissorRects[index] = camera->GetScissorRect();
}

std::vector<D3D12_VIEWPORT>& CameraHolder::GetViewPorts()
{
	return mViewPorts;
}

std::vector<D3D12_RECT>& CameraHolder::GetScissorRects()
{
	return mScissorRects;
}

std::shared_ptr<ConstantBufferObject> CameraHolder::GetCamerasBuffer()
{
	return mCamerasBuffer;
}

void CameraHolder::UpdateControllerHeap()
{
	std::list<std::list<DrawController3D*>::iterator> deleteIterators;
	auto endItr = mControllers.end();
	for (auto itr = mControllers.begin(); itr != endItr; ++itr)
	{
		if ((*itr) != nullptr)
		{
			(*itr)->UpdateDescriptorHeap();
		}
		else
		{
			deleteIterators.push_back(itr);
		}
	}

	for (auto dItr : deleteIterators)
	{
		mControllers.erase(dItr);
	}
}

void CameraHolder::UpdateBuffers()
{
	mCamerasBuffer->WriteBuffer256Alignment(&mMultiCameras, sizeof(mMultiCameras), 1);
}
