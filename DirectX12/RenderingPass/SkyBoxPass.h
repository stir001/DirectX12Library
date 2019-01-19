#pragma once
#include "Base/RenderingPassObject.h"

class SkyBox;
class Dx12DescriptorHeapObject;
class RendertargetObject;
class CameraHolder;
struct ID3D12Device;
struct SkyBoxTextures;

class SkyBoxPass :
	public RenderingPassObject
{
public:
	SkyBoxPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		std::shared_ptr<RendertargetObject> rendertarget,
		int width, int height, SkyBoxTextures& textures);
	SkyBoxPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		std::shared_ptr<RendertargetObject> rendertarget,
		int width, int height);
	~SkyBoxPass();

	/**
	*	�e�t���[���̈�ԍŏ��Ɏ��s�����
	*/
	 void FirstUpdate();

	/**
	*	���̃p�X��ExecutePath���Ă΂�钼�O�Ɏ��s�����
	*	��{�I�ɂ����ŃR�}���h���X�g��close���Ă�
	*/
	 void PreExecuteUpdate();

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	 void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExecutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	 void ResetCommandList();

	/**
	*	�R�}���h���X�g���擾����
	*/
	 std::shared_ptr<Dx12CommandList> GetCommandList();

	 /**
	 *	�ŏI�����_�����O���ʂ�Ԃ��֐��@��ԍŌ�̃p�X�̂ݕK�{����ȊO�͎������Ȃ��Ă�����
	 */
	 virtual std::shared_ptr<Dx12BufferObject> GetRenderTarget();

	 /**
	 *	@brief	SkyBox�̃e�N�X�`����ݒ肷��
	 *	@param[in]	tex	�ݒ肵���e�N�X�`��
	 */
	 void SetSkyBoxTextures(SkyBoxTextures& tex);
private:
	std::shared_ptr<SkyBox> mSkyBox;
	std::shared_ptr<RendertargetObject> mRendertarget;
	std::shared_ptr<CameraHolder> mHolder;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	int mWndWidth;
	int mWndHeight;
};

