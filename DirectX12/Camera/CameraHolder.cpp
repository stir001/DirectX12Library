#include "stdafx.h"
#include "CameraHolder.h"
#include "Camera/Dx12Camera.h"
#include "DrawObject/DrawController3D.h"

CameraHolder::CameraHolder()
{
}

CameraHolder::~CameraHolder()
{
}

std::shared_ptr<Dx12Camera> CameraHolder::CreateCamera()
{
	return std::shared_ptr<Dx12Camera>();
}

std::shared_ptr<Dx12Camera> CameraHolder::GetCamera(unsigned int index) const
{
	if (index >= static_cast<unsigned int>(mCameras.size())) return nullptr;
	return mCameras[index];
}

void CameraHolder::DeleteCamera(unsigned int index)
{
	auto itr = mCameras.begin() + index;
	mCameras.erase(itr);
	mCameras.shrink_to_fit();
}

void CameraHolder::SetCameraBuffer(std::shared_ptr<DrawController3D> controller)
{
	controller->SetCameraBuffer(mCamerasBuffer);
	mControllers.push_back(controller);
}

void CameraHolder::UpdateControllerHeap()
{
	for (auto& ctrl : mControllers)
	{
		ctrl.expired();
	}
}
