#pragma once
/**
*	@addtogroup Dx12CommandList
*	@file Dx12CommandList.h
*	@brief Dx12GraphicsCommandList�����b�v�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include <vector>

class Dx12BufferObject;
class Dx12DescriptorHeapObject;
class DrawObjectController;

/**
*	@ingroup Dx12CommandList
*	@class Dx12CommandList
*	@brief ID3D12GraphicsCommandList�����b�v�����N���X
*/
class Dx12CommandList
{
public:
	/**
	*	@param	name	�R�}���h���X�g�̖��O
	*	@param	dev		ID3D12Device�̎Q��
	*	@param	type	�R�}���h���X�g�̃^�C�v
	*/
	Dx12CommandList(const std::string& name 
		, const Microsoft::WRL::ComPtr<ID3D12Device>& dev 
		, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	virtual ~Dx12CommandList();
	
	/**
	*	@brief	�R�}���h���X�g���擾����
	*	@return �ێ����Ă���R�}���h���X�g
	*/
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const;

	/**
	*	@brief	�R�}���h���X�g�̃^�C�v�𓾂�
	*	@return	�R�}���h���X�g�^�C�v
	*/
	D3D12_COMMAND_LIST_TYPE GetType() const;

	/**
	*	@brief	�R�}���h���X�g�����Z�b�g����
	*	@return	���Z�b�g�̌���
	*/
	virtual HRESULT Reset();

	/**
	*	@brief	�R�}���h���X�g��C�ӂ�PipelineState���g���ă��Z�b�g����
	*	@param[in]	pipelineState	���Z�b�g���Ɏg�p����pipelinestate
	*	@return	���Z�b�g�̌���
	*/
	virtual HRESULT Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState);

	/**
	*	@brief	CommandList�ɔC�ӂ�DescriptorHeap���Z�b�g����
	*	@param[in]	descHeap	�Z�b�g����DescriptorHeap�����b�v��������
	*	@return	����
	*/
	HRESULT SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const;

	/**
	*	@brief	CommandList�ɔC�ӂ�DescriptorHeap���Z�b�g����
	*	@param[in]	descHeap	�Z�b�g����DescriptorHeap
	*	@return	����
	*/
	HRESULT SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descHeap) const;

	/**
	*	@brief	DescriptorHeap�̔C�ӂ̏ꏊ���e�[�u���ɃZ�b�g����
	*	@param[in]	descHeap	�Z�b�g�������v�f������DescriptorHeap
	*	@param[in]	resourceIndex	�Z�b�g������DescriptorHeap��Index
	*	@param[in]	rootparamIndex	�Z�b�g����e�[�u���̃C���f�b�N�X
	*	@return	����
	*/
	HRESULT SetGraphicsRootDescriptorTabel(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootparamIndex) const;

	/**
	*	@brief	DescriptorHeap��gpuHandle�̏ꏊ���e�[�u���ɃZ�b�g����
	*	@param[in]	rootparamaterIndex	�Z�b�g����e�[�u���̃C���f�b�N�X
	*	@param[in]	gpuHandle			DescriptorHeap�̃Z�b�g�������ꏊ��gpuHandle
	*	@return	����
	*/
	HRESULT SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) const;

	/**
	*	@brief	TransitionBarrier���͂�
	*	@param[in]	resource	�o���A�𒣂肽�����\�[�X
	*	@param[in]	transitionState	�ǂ̃X�e�[�g�Ƀg�����W�V�������邩�̃X�e�[�g
	*	@return	����
	*/
	HRESULT TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const;
	
	/**
	*	@brief	DepthStencil���w��̒l�Ŗ��߂�
	*	@param[in]	depthCpuHandle	DepthStencilView�̂���cpuHandle
	*	@param[in]	clearValue		���߂����l
	*	@param[in]	rect			�ǂ͈̔͂𖄂߂邩
	*	@param[in]	rectNum			����Rect��n������
	*/
	void ClearDepthView(D3D12_CPU_DESCRIPTOR_HANDLE depthCpuHandle, float clearValue ,D3D12_RECT* rect, unsigned int rectNum = 0);

	/**
	*	@brief	DepthStencil���w��̒l�Ŗ��߂�
	*	@param[in]	depthCpuHandle	DepthStencilView�̂���cpuHandle
	*	@param[in]	clearValue		���߂����l
	*	@param[in]	rect			�ǂ͈̔͂𖄂߂邩
	*	@param[in]	rectNum			����Rect��n������
	*/
	void ClearStencilView(D3D12_CPU_DESCRIPTOR_HANDLE stencilCpuHandle, unsigned int clearValue, D3D12_RECT* rect, unsigned int rectNum = 0);

	/**
	*	@brief	RenderTarget���Z�b�g����
	*	@param[in]	renderTargetCount	�Ȃ�RenderTarget�����邩
	*	@param[in]	renderTargetCpuHandle	RenderTargetView������Descriptor��CPUHandle
	*	@param[in]	depthCpuHandle		�����_�[�^�[�Q�b�g�ɏ������ނƂ��Ɏg��DepthSrencilView�̂���DescriptorHeap��CPUHandle
	*/
	void OMSetRenderTargets(unsigned int renderTargetCount, D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE* depthCpuHandle = nullptr);

	/**
	*	@brief	�����_�[�^�[�Q�b�g���w��̒l�Ŗ��߂�clear����
	*	@param[in]	rendertargetCpuHandle	RenderTaregetView�̂���DescriptorHeap��CpuHandle
	*	@param[in]	colorRGBA		���߂�l
	*	@param[in]	rect 	�ǂ͈̔͂𖄂߂邩
	*	@param[in]	rectNum	Rect�����n������
	*/
	void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, const float colorRGBA[4], D3D12_RECT* rect, unsigned int rectNum = 0);

	/**
	*	@brief ViewPort���Z�b�g����
	*	@param[in]	viewPorts	�Z�b�g����ViewPort
	*	@param[in]	viewPortsCount	ViewPort�̑���
	*/
	void RSSetViewports(D3D12_VIEWPORT* viewPorts, unsigned int viewportCount = 1);

	/**
	*	@brief	�V�U�[���N�g���Z�b�g����
	*	@param[in]	scissorRects	�Z�b�g����V�U�[���N�g
	*	@param[in]	scissorRectCount�V�U�[���N�g�̑���
	*/
	void RSSetScissorRects(D3D12_RECT* scissorRects, unsigned int scissorRectsCount = 1);

	/**
	*	@brief	�R�}���h���X�g�̖��O�𓾂�
	*	@return	�R�}���h���X�g�̖��O
	*/
	const std::string& GetName() const;

	/**
	*	�R�}���h���X�g�����
	*/
	void Close() const;

	void SetDrawController(std::shared_ptr<DrawObjectController> controller);
private:
	/**
	*	�R�}���h���X�g�̖��O
	*/
	std::string mName;

	/**
	*	�ێ����Ă���R�}���h���X�g
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;

	/**
	*	�ێ����Ă���R�}���h�A���P�[�^�[
	*/
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdallcator;

	/**
	*	�R�}���h���X�g�̃^�C�v
	*/
	const D3D12_COMMAND_LIST_TYPE mType;

	std::vector<std::shared_ptr<DrawObjectController>> mControllers;

	/**
	*	@brief	ID3D12Deviec��Remove���ꂽ���ǂ������`�F�b�N����
	*	@return	����
	*/
	HRESULT GetDeviceRemoveReason() const;
};

