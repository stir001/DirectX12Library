#pragma once

#include <vector>
#include <list>
#include <memory>
#include <d3d12.h>

class Dx12Camera;
class DrawController3D;
class ConstantBufferObject;

class CameraHolder
{
public:
	CameraHolder();
	~CameraHolder();

	std::shared_ptr<Dx12Camera> CreateCamera();
	std::shared_ptr<Dx12Camera> GetCamera(unsigned int index) const;
	void DeleteCamera(unsigned int index);
	void SetCameraBuffer(std::shared_ptr<DrawController3D> controller);
private:
	std::shared_ptr<ConstantBufferObject> mCamerasBuffer;
	std::list<std::weak_ptr<DrawController3D>> mControllers;
	std::vector<std::shared_ptr<Dx12Camera>> mCameras;
	std::vector<D3D12_VIEWPORT> mViewPorts;
	std::vector<D3D12_RECT> mRect;

	void UpdateControllerHeap();
};

