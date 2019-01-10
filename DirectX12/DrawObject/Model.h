#pragma once
/**
*	@addtogroup Model
*	@file Model.h
*	@brief 3Dモデルクラスの基底クラスを定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/20
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
*	@brief 扱うマテリアル情報を定義
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
*	@brief 3Dモデルを扱うときに使う関数、変数を定義
*/
class Model
{
public:
	Model();
	virtual ~Model();

	/**
	*	@brief	IndexBufferをGraphicsCommandListに設定する
	*	@param[in]	cmdList	IndexBufferを設定するGraphicsCommandList
	*/
	void SetIndexBuffer(const std::shared_ptr<Dx12CommandList>& cmdList) const;

	/**
	*	@brief	VertexBufferをGraphicsCommandListに設定する
	*	@param[in]	cmdList	VertexBufferを設定するGraphicsCommnadList
	*/
	void SetVertexBuffer(const std::shared_ptr<Dx12CommandList>& cmdList) const;

	/**
	*	@brief	IndexBufferを取得する
	*	@return	使用しているIndexBuffer
	*/
	std::shared_ptr<IndexBufferObject> GetIndexBuffer() const;

	/**
	*	@brief	VertexBufferを取得する
	*	@return	使用しているVertexBuffer
	*/
	std::shared_ptr<VertexBufferObject> GetVertexBuffer() const;

	/**
	*	@brief	保持しているテクスチャ情報を持つvectorを取得する
	*	@return	保持しているテクスチャ群
	*/
	std::vector<std::shared_ptr<TextureObject>> GetTextureObjects() const;

	/**
	*	@brief	マテリアル情報を保持しているバッファを取得する
	*	@return	マテリアル情報をもつConstantBuffer
	*/
	std::shared_ptr<ConstantBufferObject> GetMaterialBuffer() const;

	/**
	*	@brief	モデルの名前を取得する
	*	@return	モデルの名前
	*/
	const std::string& GetModelName() const;

	/**
	*	@brief	モデルをロードしたときに指定したファイルパスを取得する
	*	@return	ファイルパス
	*/
	const std::string& GetModelPath() const;
protected:
	/**
	*	インデックス情報書き込み用のバッファ
	*/
	std::shared_ptr<IndexBufferObject> mIndexBuffer;

	/**
	*	頂点情報書き込み用のバッファ
	*/
	std::shared_ptr<VertexBufferObject> mVertexBuffer;

	/**
	*	テクスチャの総数
	*/
	unsigned int mTexturecount;

	/**
	*	テクスチャ情報
	*/
	std::vector<std::shared_ptr<TextureObject>> mTextureObjects;

	/**
	*	マテリアル情報書き込みようバッファ
	`*/
	std::shared_ptr<ConstantBufferObject> mMaterialBuffer;

	/**
	*	マテリアル情報保持用
	*/
	std::vector<Dx12Material> mD12mat;

	/**
	*	モデルの名前
	*/
	std::string mModelName;

	/**
	*	モデルをロードしたときに指定したファイルパス
	*/
	std::string mModelPath;
private:
};

