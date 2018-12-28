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
class RenderingPassObject;


enum class DefaultPass
{
	BackGround,
	Model,
	UI,
	Max
};

/**
*�����_�����O�p�X�����ԂɌĂ΂�邱�Ƃ�ۏ؂���
*���ׂẴp�X���I��������R�}���h���X�g�����Z�b�g����
*/
class RenderingPassManager
{
public:
	~RenderingPassManager();

	static RenderingPassManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new RenderingPassManager();
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
	*	DeleteRenderingpass���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X������邪�R�}���h���X�g���͕̂ω����Ȃ�
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPassCommandList(unsigned int passIndex) const;

	/**
	*	�p�X�̖��O���炻�̃p�X�̃R�}���h��ςނׂ��R�}���h���X�g���擾����
	*	DeleteRenderingpass���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X������邪�R�}���h���X�g���͕̂ω����Ȃ�
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPassCommandList(const std::string& passName) const;

	/**
	*	�p�X�̖��O���炻�̃p�X�̃C���f�b�N�X���擾����
	*	�����_�����O�p�X�̃C���f�b�N�X���擾����
	*	DeleteRenderingpass���\�b�h���Ă񂾌ゾ�ƃC���f�b�N�X�������̂Œ���
	*/
	unsigned int GetRenderingPassIndex(const std::string& passName) const;


	/**
	*	�p�X��ǉ�����
	*/
	bool AddRenderingPass(const std::shared_ptr<RenderingPassObject>& passObj, unsigned int& out_PassIndex);

	/**
	*	@brief	�p�X���w��̏ꏊ�ɑ}������
	*/
	bool InsertRenderingPass(std::shared_ptr<RenderingPassObject>& passObj, unsigned int insertPassIndex);

	/**
	*	�w�肵�������_�����O�p�X�̃R�}���h���X�g���p�X�ォ��폜����
	*	��������s������̓p�X�̃C���f�b�N�X�������̂Œ���
	*/
	bool DeleteRenderingPass(unsigned int passIndex);
	bool DeleteRenderingPass(const std::string& passName);

	/**
	*	�p�X�̑S�̂̐����擾
	*/
	unsigned int GetNumCuurentPass() const;

	/**
	*	���݂̃X���b�v�`�F�C���̃����_�[�^�[�Q�b�g�̃q�[�v�n���h�����擾����
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeapHandle() const;

	/**
	*	���݂̃X���b�v�`�F�C���̃����_�[�^�[�Q�b�g�̃q�[�v���擾����
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> GetCurrentRTVDescHeap() const;

	/**
	*	���݂̃X���b�v�`�F�C���̃����_�[�^�[�Q�b�g�ɂ��擾����
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTarget() const;

	/**
	*	�p�X�̃A�N�e�B�u�A��A�N�e�B�u��ݒ肷��
	*	@param[in]	renderingpass	�����_�����O�p�X�̔ԍ�
	*	@param[in]	isActive	ture:�A�N�e�B�u	false:��A�N�e�B�u
	*/
	void SetIsActiveValue(unsigned int renderingpass, bool isActive);

	/**
	*	�p�X�̃A�N�e�B�u�A��A�N�e�B�u��ݒ肷��
	*	@param[in]	passName	�����_�����O�p�X�̖��O
	*	@param[in]	isActive	ture:�A�N�e�B�u	false:��A�N�e�B�u
	*/
	void SetIsActiveValue(std::string passName, bool isActive);

	/**
	*	���ׂẴp�X���폜����
	*/
	void AllPassClear();

private:
	RenderingPassManager();
	RenderingPassManager(const RenderingPassManager&) = delete;
	RenderingPassManager(const RenderingPassManager&&) = delete;
	RenderingPassManager& operator=(const RenderingPassManager&) = delete;
	RenderingPassManager& operator=(const RenderingPassManager&&) = delete;

	static RenderingPassManager* mInstance;

	std::vector<std::shared_ptr<RenderingPassObject>> mRenderingPassObjects;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mRenderCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mRenderCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;

	std::shared_ptr<SwapChainObject> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int AddRenderingPassObject(const std::shared_ptr<RenderingPassObject>& passObj);

	void WaitCmdQueue();
	void CopyLastPassRenderTarget();
};