#pragma once
#include "CameraBufferStructure.h"

#include <vector>
#include <list>
#include <memory>
#include <d3d12.h>
#include <wrl.h>

class Dx12Camera;
class DrawController3D;
class ConstantBufferObject;

class CameraHolder : public std::enable_shared_from_this<CameraHolder>
{
public:
	CameraHolder(int wndWidth, int wndHeight, Microsoft::WRL::ComPtr<ID3D12Device> dev);
	~CameraHolder();

	std::shared_ptr<Dx12Camera> CreateCamera(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, 
		D3D12_VIEWPORT viewport, D3D12_RECT scissorRect);
	std::shared_ptr<Dx12Camera> GetCamera(unsigned int index) const;
	int GetCameraNum() const;
	bool DeleteCamera(unsigned int index);
	void SetCameraBuffer(DrawController3D* controller);
	void SetCameraElement(Dx12Camera* camera);
	void SetCameraViewPort(Dx12Camera* camera);
	void SetCameraScissorRect(Dx12Camera* camera);
	std::vector<D3D12_VIEWPORT>& GetViewPorts();
	std::vector<D3D12_RECT>& GetScissorRects();
	std::shared_ptr<ConstantBufferObject> GetCamerasBuffer();
private:
	const static unsigned int MAXCAMERA_NUM = 4;
	struct MultiCameras
	{
		Dx12CameraBuffer cameraBuffer[MAXCAMERA_NUM];
		unsigned int cameraNum;
		MultiCameras() :cameraNum(0), cameraBuffer() {}
	};

	MultiCameras mMultiCameras;
	DirectX::XMINT2 mWndSize;
	std::shared_ptr<ConstantBufferObject> mCamerasBuffer;
	std::list<DrawController3D*> mControllers;
	std::vector<std::shared_ptr<Dx12Camera>> mCameras;
	std::vector<D3D12_VIEWPORT> mViewPorts;
	std::vector<D3D12_RECT> mScissorRects;
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;

	void UpdateControllerHeap();
	void UpdateBuffers();
};

