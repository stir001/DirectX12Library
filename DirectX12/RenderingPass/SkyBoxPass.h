#pragma once
#include "Base/RenderingPassObject.h"
class SkyBoxPass :
	public RenderingPassObject
{
public:
	SkyBoxPass();
	~SkyBoxPass();

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
	 std::shared_ptr<Dx12CommandList> GetCommandList();
};

