#pragma once
#include <wrl.h>
#include <memory>
#include <string>

struct ID3D12CommandQueue;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

class Dx12CommandList;

class RenderingPassObject
{
public:
	RenderingPassObject(const std::string& pathName);
	virtual ~RenderingPassObject();

	/**
	*	�e�t���[���̈�ԍŏ��Ɏ��s�����
	*/
	virtual void FirstUpdate() = 0;	

	/**
	*	���̃p�X��ExcutePath���Ă΂�钼�O�Ɏ��s�����
	*	��{�I�ɂ����ŃR�}���h���X�g��close���Ă�
	*/
	virtual void PreExcuteUpdate() = 0;

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	virtual void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue) = 0;

	/**
	*	ExcutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	virtual void ResetCommandList() = 0;

	/**
	*	�R�}���h���X�g���擾����
	*/
	virtual Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() = 0;

	/**
	*	�ŏI�����_�����O���ʂ�Ԃ��֐��@��ԍŌ�̃p�X�̂ݕK�{����ȊO�͎������Ȃ��ł�����
	*/
	virtual Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTarget();

	/**
	*	���̃p�X��L���ɂ��邩�����ɂ��邩�̐ݒ���s��
	*/
	virtual void SetActive(bool isActive);

	/**
	*	���̃p�X���L������������Ԃ�:true �L��, false ����
	*/
	virtual bool IsActive() const;

	const std::string& GetPassName() const;
protected:
	std::shared_ptr<Dx12CommandList> mCmdList;
	std::string mPassName;
private:
	bool mIsActive;
};

