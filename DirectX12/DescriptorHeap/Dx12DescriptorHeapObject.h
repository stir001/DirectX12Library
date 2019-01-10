#pragma once
/**
*	@addtogroup	Dx12DescriptorHeap
*	@file Dx12DescriptorHeapObject
*	@brief ID3D12DescriptorHeap�����b�v���ێ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/12
*/

#include <wrl.h>
#include <vector>
#include <memory>
#include <d3d12.h>

struct ID3D12DescriptorHeap;
struct ID3D12Device;
class Dx12BufferObject;
class Dx12CommandList;

/**
*	@ingroup Dx12DescriptorHeap
*	@class Dx12DescriptorHeapObject
*	@brief ID3D12DescriptorHeap��ێ�����
*/
class Dx12DescriptorHeapObject : public std::enable_shared_from_this<Dx12DescriptorHeapObject>
{
public:
	/**
	*	@param[in]	name		DescriptorHeap�̖��O
	*	@param[in]	dev			ID3D12Device�̎Q��
	*	@param[in]	buffers		DescriptorHeap�ɃZ�b�g����Buffer��vector
	*	@param[in]	heapType	DescriptorHeap�̃^�C�v
	*/
	Dx12DescriptorHeapObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::vector<std::shared_ptr<Dx12BufferObject>>& buffers, D3D12_DESCRIPTOR_HEAP_TYPE heapType);
	virtual ~Dx12DescriptorHeapObject();

	/**
	*	@brief	DescriptorHeap���擾����
	*	@return	�ێ����Ă���DescriptorHeap
	*/
	virtual Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap()const;

	/**
	*	@brief	GetCPUDescriptorHandleForHeapStart()�̃��b�v�֐�
	*	@return	DescriptorHeap��CPU�n���h���̍ŏ��̈ʒu
	*/
	virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHeapHandleStart() const;

	/**
	*	@brief	GetGPUDescriptorHandleForHeapStart()�̃��b�v�֐�
	*	@return	DescriptorHeap��GPU�n���h���̍ŏ��̈ʒu
	*/
	virtual D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHeapHandleStart() const;

	/**
	*	@brief	DescriptorHeap���R�}���h���X�g�ɃZ�b�g����
	*	@param[in]	cmdList	DescriptorHeap���Z�b�g����R�}���h���X�g
	*/
	virtual void SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;

	/**
	*	@brief	DescriptorHeap���R�}���h���X�g�ɃZ�b�g����
	*	@param[in]	cmdList	DescriptorHeap���Z�b�g����R�}���h���X�g
	*/
	virtual void SetDescriptorHeap(const std::shared_ptr<Dx12CommandList>& cmdList) const;

	/**
	*	@brief	�R�}���h���X�g��DescriptorHeapTable��ݒ肷��
	*	@param[in]	resourceHandleIndex	DescriptorTable�Ɋi�[���Ă��郊�\�[�X�n���h���̃C���f�b�N�X
	*	@param[in]	rootParamaterIndex	���\�[�X���o�C���h����rootParamaterIndex
	*	@param[in]	handleOffsetCount	resourceHandle�̃I�t�Z�b�g(���ڂ��g�p���邩)
	*/
	virtual void SetGraphicsDescriptorTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceHandleIndex, unsigned int rootParamaterIndex, unsigned int handleOffsetCount = 0) const;

	/**
	*	@brief	�R�}���h���X�g��DescriptorHeapTable��ݒ肷��
	*	@param[in]	resourceHandleIndex	DescriptorTable�Ɋi�[���Ă��郊�\�[�X�n���h���̃C���f�b�N�X
	*	@param[in]	rootParamaterIndex	���\�[�X���o�C���h����rootParamaterIndex
	*	@param[in]	handleOffsetCount	resourceHandle�̃I�t�Z�b�g(���ڂ��g�p���邩)
	*/
	virtual void SetGraphicsDescriptorTable(const std::shared_ptr<Dx12CommandList> cmdList, unsigned int resourceHandleIndex, unsigned int rootParamaterIndex, unsigned int handleOffsetCount = 0) const;
protected:
	/**
	*	�ێ����Ă���DescriptorHeap
	*/
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescHeap;
private:
	/**
	*	DescriptorHeap�Ɗ֘A�Â����Ă��郊�\�[�X�̏��
	*/
	struct ResourceBind
	{
		std::weak_ptr<Dx12BufferObject> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	};

	/**
	*	HeapType�ɉ������C���N�������g�̒l
	*/
	UINT mHeapIncrementSize;

	/**
	*	���̃N���X�Ɗ֘A�t�����Ă���Resource�̏��
	*/
	std::vector<ResourceBind> mResourceBinds;
};

