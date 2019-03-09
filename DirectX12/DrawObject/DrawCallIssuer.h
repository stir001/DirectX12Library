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
	*	@brief	描画命令を発行する
	*	@param[in]	cmdList	命令を積むコマンドリスト
	*/
	virtual void IssueDrawCall(std::shared_ptr<Dx12CommandList>& cmdList) = 0;

	/**
	*	@brief	バンドルを更新する
	*/
	virtual void UpdateBundle() = 0;

	/**
	*	@brief	インスタンスの数を上げる
	*/
	virtual unsigned int CountUpInstanceNum();

	/**
	*	@brief	インスタンス数を取得する
	*	@return	インスタンス数
	*/
	virtual unsigned int GetInstanceNum() const;

	/**
	*	@brief	インスタンス数を0にする
	*/
	virtual void ResetInstanceNum();

	/**
	*	@brief	前フレームのInstance数を取得する
	*	@return	前フレームのInstance数
	*/
	virtual unsigned int GetPreInstanceNum() const;
protected:
	/**
	*	使用するバンドルコマンドリスト
	*/
	std::shared_ptr<Dx12CommandList> mBundleCmdList;
private:
	/**
	*	インスタンス数
	*/
	unsigned int mInstanceNum;

	/**
	*	前フレームのインスタンス数
	*/
	unsigned int mPreInstanceNum;
};

