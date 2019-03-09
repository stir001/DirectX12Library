#pragma once
#include <memory>

class Dx12CommandList;
class VertexBufferObject;
class IndexBufferObject;
class PipelineStateObject;
class RootSignatureObject;
class ShaderResourceObject;
class ConstantBufferObject;
class DescriptorHeapObject;

class DrawCallIssuer
{
public:
	DrawCallIssuer();
	virtual ~DrawCallIssuer();

	/**
	*	@brief	�`�施�߂𔭍s����
	*	@param[in]	cmdList	���߂�ςރR�}���h���X�g
	*/
	virtual void IssueDrawCall(std::shared_ptr<Dx12CommandList>& cmdList) = 0;

	/**
	*	@brief	�o���h�����X�V����
	*/
	virtual void UpdateBundle() = 0;

	/**
	*	@brief	�C���X�^���X�̐����グ��
	*/
	virtual unsigned int CountUpInstanceNum();

	/**
	*	@brief	�C���X�^���X�����擾����
	*	@return	�C���X�^���X��
	*/
	virtual unsigned int GetInstanceNum() const;

	/**
	*	@brief	�C���X�^���X����0�ɂ���
	*/
	virtual void ResetInstanceNum();

	/**
	*	@brief	�O�t���[����Instance�����擾����
	*	@return	�O�t���[����Instance��
	*/
	virtual unsigned int GetPreInstanceNum() const;
protected:
	/**
	*	�g�p����o���h���R�}���h���X�g
	*/
	std::shared_ptr<Dx12CommandList> mBundleCmdList;
private:
	/**
	*	�C���X�^���X��
	*/
	unsigned int mInstanceNum;

	/**
	*	�O�t���[���̃C���X�^���X��
	*/
	unsigned int mPreInstanceNum;
};

