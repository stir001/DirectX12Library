#pragma once
/**
*	@addtogroup ImageObject
*	@file ImageObject.h
*	@brief 2D�摜����ێ�����N���X�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/20
*/
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>
#include <string>

class TextureObject;
class ShaderResourceObject;

struct ImageVertex;

/**
*	@ingroup ImageObjcet
*	@class ImageObjcet
*	@brief 2D�摜����ێ�����
*/
class ImageObject
{
	friend class ImageLoader;
public:
	~ImageObject();

	/**
	*	@brief	�摜�̏c�������擾����
	*	@return	�摜�̏c����(�s�N�Z���P��)
	*/
	DirectX::XMFLOAT2 GetImageSize();

	/**
	*	@brief	��f��񂪏������܂�Ă���ShaderResourceObjcet���擾����
	*	@return	��f��񂪏������܂�Ă���ShaderResource
	*/
	std::shared_ptr<ShaderResourceObject> GetShaderResource() const;

	/**
	*	@brief	�e�N�X�`���̖��O���擾����
	*	@return	�e�N�X�`���̖��O
	*/
	const std::string& GetTextureName() const;

	/**
	*	@brief	�ǂݍ��񂾎��Ɏw�肳�ꂽ�t�@�C���p�X���擾����
	*	@return	�t�@�C���p�X
	*/
	const std::string& GetFilePath() const;

	/**
	*	@brief	�K���}�l���擾����
	*	@return	�K���}�l
	*/
	float GetGamma() const;
private:
	/**
	*	@param[in]	width	2D�摜�̉���
	*	@param[in]	height	2D�摜�̏c��
	*/
	ImageObject(int width, int height, std::shared_ptr<TextureObject> texObj);

	/**
	*	@enum	eROOT_PARAMATER_INDEX
	*	@brief	���̃N���X�Ŏg�p����RootParamaterIndex��enum
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE
	};

	/**
	*	�摜�̉���
	*/
	int mWidth;

	/**
	*	�摜�̏c��
	*/
	int mHeight;

	/**
	*	�摜����ێ�
	*/
	std::shared_ptr<TextureObject> mTexObj;

	static std::shared_ptr<ImageObject> Create(int width, int height, std::shared_ptr<TextureObject> texObj);
};

