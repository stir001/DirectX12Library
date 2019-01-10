#pragma once
/**
*	@addtogroup Model
*	@file Model.h
*	@brief 3D���f���N���X�̊��N���X���`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/20
*/
#include <vector>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

class IndexBufferObject;
class VertexBufferObject;
class TextureObject;
class ConstantBufferObject;
class Dx12CommandList;
class DrawObjectLoader;

/**
*	@ingroup Model
*	@struct Dx12Material
*	@brief �����}�e���A�������`
*/
struct Dx12Material
{
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT4 ambient;
	float alpha;
	float specularity;
};

/**
*	@ingroup Model
*	@class Model
*	@brief 3D���f���������Ƃ��Ɏg���֐��A�ϐ����`
*/
class Model
{
public:
	Model();
	virtual ~Model();

	/**
	*	@brief	IndexBuffer��GraphicsCommandList�ɐݒ肷��
	*	@param[in]	cmdList	IndexBuffer��ݒ肷��GraphicsCommandList
	*/
	void SetIndexBuffer(const std::shared_ptr<Dx12CommandList>& cmdList) const;

	/**
	*	@brief	VertexBuffer��GraphicsCommandList�ɐݒ肷��
	*	@param[in]	cmdList	VertexBuffer��ݒ肷��GraphicsCommnadList
	*/
	void SetVertexBuffer(const std::shared_ptr<Dx12CommandList>& cmdList) const;

	/**
	*	@brief	IndexBuffer���擾����
	*	@return	�g�p���Ă���IndexBuffer
	*/
	std::shared_ptr<IndexBufferObject> GetIndexBuffer() const;

	/**
	*	@brief	VertexBuffer���擾����
	*	@return	�g�p���Ă���VertexBuffer
	*/
	std::shared_ptr<VertexBufferObject> GetVertexBuffer() const;

	/**
	*	@brief	�ێ����Ă���e�N�X�`����������vector���擾����
	*	@return	�ێ����Ă���e�N�X�`���Q
	*/
	std::vector<std::shared_ptr<TextureObject>> GetTextureObjects() const;

	/**
	*	@brief	�}�e���A������ێ����Ă���o�b�t�@���擾����
	*	@return	�}�e���A����������ConstantBuffer
	*/
	std::shared_ptr<ConstantBufferObject> GetMaterialBuffer() const;

	/**
	*	@brief	���f���̖��O���擾����
	*	@return	���f���̖��O
	*/
	const std::string& GetModelName() const;

	/**
	*	@brief	���f�������[�h�����Ƃ��Ɏw�肵���t�@�C���p�X���擾����
	*	@return	�t�@�C���p�X
	*/
	const std::string& GetModelPath() const;
protected:
	/**
	*	�C���f�b�N�X��񏑂����ݗp�̃o�b�t�@
	*/
	std::shared_ptr<IndexBufferObject> mIndexBuffer;

	/**
	*	���_��񏑂����ݗp�̃o�b�t�@
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	�e�N�X�`���̑���
	*/
	unsigned int mTexturecount;

	/**
	*	�e�N�X�`�����
	*/
	std::vector<std::shared_ptr<TextureObject>> mTextureObjects;

	/**
	*	�}�e���A����񏑂����݂悤�o�b�t�@
	`*/
	std::shared_ptr<ConstantBufferObject> mMaterialBuffer;

	/**
	*	�}�e���A�����ێ��p
	*/
	std::vector<Dx12Material> mD12mat;

	/**
	*	���f���̖��O
	*/
	std::string mModelName;

	/**
	*	���f�������[�h�����Ƃ��Ɏw�肵���t�@�C���p�X
	*/
	std::string mModelPath;
private:
};

