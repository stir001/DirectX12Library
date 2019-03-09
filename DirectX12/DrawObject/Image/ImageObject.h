#pragma once
/**
*	@addtogroup ImageObject
*	@file ImageObject.h
*	@brief 2D画像情報を保持するクラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/20
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
*	@brief 2D画像情報を保持する
*/
class ImageObject
{
	friend class ImageLoader;
public:
	~ImageObject();

	/**
	*	@brief	画像の縦横幅を取得する
	*	@return	画像の縦横幅(ピクセル単位)
	*/
	DirectX::XMFLOAT2 GetImageSize();

	/**
	*	@brief	画素情報が書き込まれているShaderResourceObjcetを取得する
	*	@return	画素情報が書き込まれているShaderResource
	*/
	std::shared_ptr<ShaderResourceObject> GetShaderResource() const;

	/**
	*	@brief	テクスチャの名前を取得する
	*	@return	テクスチャの名前
	*/
	const std::string& GetTextureName() const;

	/**
	*	@brief	読み込んだ時に指定されたファイルパスを取得する
	*	@return	ファイルパス
	*/
	const std::string& GetFilePath() const;

	/**
	*	@brief	ガンマ値を取得する
	*	@return	ガンマ値
	*/
	float GetGamma() const;
private:
	/**
	*	@param[in]	width	2D画像の横幅
	*	@param[in]	height	2D画像の縦幅
	*/
	ImageObject(int width, int height, std::shared_ptr<TextureObject> texObj);

	/**
	*	@enum	eROOT_PARAMATER_INDEX
	*	@brief	このクラスで使用するRootParamaterIndexのenum
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE
	};

	/**
	*	画像の横幅
	*/
	int mWidth;

	/**
	*	画像の縦幅
	*/
	int mHeight;

	/**
	*	画像情報を保持
	*/
	std::shared_ptr<TextureObject> mTexObj;

	static std::shared_ptr<ImageObject> Create(int width, int height, std::shared_ptr<TextureObject> texObj);
};

