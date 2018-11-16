#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include <dxgi1_4.h>
#include <functional>

class SwapChainObject;
class Dx12DescriptorHeapObject;
class RenderingPathObject;


/**
*�����_�����O�p�X�����ԂɌĂ΂�邱�Ƃ�ۏ؂���
*���ׂẴp�X���I��������R�}���h���X�g�����Z�b�g����
*/
class RenderingPathManager
{
public:
	~RenderingPathManager();

	static RenderingPathManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new RenderingPathManager();
		}
		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/**
	*	RenderingManager�̏���������
	*/
	void Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd);

	void Render();

	/**
	*	�p�X�ԍ���0����
	*	�p�X�̃C���f�b�N�X���炻�̃p�X�̃R�}���h��ςނׂ��R�}���h���X�g���擾����
	*	DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X������邪�R�}���h���X�g���͕̂ω����Ȃ�
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex) const;

	/**
	*	�p�X�̖��O���炻�̃p�X�̃R�}���h��ςނׂ��R�}���h���X�g���擾����
	*	DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X������邪�R�}���h���X�g���͕̂ω����Ȃ�
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName) const;

	/**
	*	�p�X�̖��O���炻�̃p�X�̃C���f�b�N�X���擾����
	*	�����_�����O�p�X�̃C���f�b�N�X���擾����
	*	DeleteRenderingPath���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X�������̂Œ���
	*/
	unsigned int GetRenderingPathIndex(const std::string& pathName) const;


	/**
	*
	*/
	bool AddRenderPath(std::shared_ptr<RenderingPathObject>& pathObj, unsigned int& out_PathIndex);

	bool InsertRenderPath(std::shared_ptr<RenderingPathObject>& pathObj, unsigned int insertPathIndex);

	/**
	*	�w�肵�������_�����O�p�X�̃R�}���h���X�g���p�X�ォ��폜����
	*	��������s������̓p�X�̃C���f�b�N�X�������̂Œ���
	*/
	bool DeleteRenderingPath(unsigned int pathIndex);
	bool DeleteRenderingPath(const std::string& pathName);

	/**
	*	�p�X�̑S�̂̐����擾
	*/
	unsigned int GetNumCuurentPath() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeapHandle() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetCurrentRTVDescHeap() const;

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTarget() const;

	void SetIsActiveValue(unsigned int renderingPath, bool isActive);
	void SetIsActiveValue(std::string pathName, bool isActive);

	void AllPathClear();

private:
	RenderingPathManager();
	RenderingPathManager(const RenderingPathManager&) = delete;
	RenderingPathManager(const RenderingPathManager&&) = delete;
	RenderingPathManager& operator=(const RenderingPathManager&) = delete;
	RenderingPathManager& operator=(const RenderingPathManager&&) = delete;

	static RenderingPathManager* mInstance;

	std::vector<std::shared_ptr<RenderingPathObject>> mRenderingPathObjects;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mRenderCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mRenderCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;

	std::shared_ptr<SwapChainObject> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int AddRenderingPathObject(std::shared_ptr<RenderingPathObject>& pathObj);

	void WaitCmdQueue();
	void CopyLastPathRenderTarget();
};