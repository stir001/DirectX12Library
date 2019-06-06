#pragma once
#include <wrl.h>
#include <memory>
#include <string>

struct ID3D12CommandQueue;

class Dx12CommandList;
class Dx12BufferObject;

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
	*	���̃p�X��ExecutePath���Ă΂�钼�O�Ɏ��s�����
	*	��{�I�ɂ����ŃR�}���h���X�g��close���Ă�
	*/
	virtual void PreExecuteUpdate() = 0;

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	virtual void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue) = 0;

	/**
	*	ExecutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	virtual void ResetCommandList() = 0;

	/**
	*	�R�}���h���X�g���擾����
	*/
	virtual std::shared_ptr<Dx12CommandList> GetCommandList() = 0;

	/**
	*	�ŏI�����_�����O���ʂ�Ԃ��֐��@��ԍŌ�̃p�X�̂ݕK�{����ȊO�͎������Ȃ��Ă�����
	*		�f�t�H���g�ł͋��shared_ptr��Ԃ�
	*/
	virtual std::shared_ptr<Dx12BufferObject> GetRenderTarget();

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

