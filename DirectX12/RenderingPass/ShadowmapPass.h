#pragma once
#include "Base/RenderingPassObject.h"
#include <memory>

class DepthBufferObject;
class LightObject;

class ShadowmapPass :
	public RenderingPassObject
{
public:
	ShadowmapPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~ShadowmapPass();

	/**
	*	�e�t���[���̈�ԍŏ��Ɏ��s�����
	*/
	void FirstUpdate();

	/**
	*	���̃p�X��ExcutePath���Ă΂�钼�O�Ɏ��s�����
	*	��{�I�ɂ����ŃR�}���h���X�g��close���Ă�
	*/
	void PreExcuteUpdate();

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExcutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	void ResetCommandList();

	/**
	*	�R�}���h���X�g���擾����
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();
private:
	std::shared_ptr<DepthBufferObject> mShadowmapDepth;
	std::shared_ptr<LightObject> mDirectionalLight;
};

