#pragma once
#include "ShaderDatasStructure.h"

#include <string>
#include <map>
#include <vector>
#include <list>

class ShaderCompiler
{
public:
	~ShaderCompiler();

	static ShaderCompiler& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new ShaderCompiler();
		}

		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
		}
		mInstance = nullptr;
	}

	/**@fn
	*渡されたシェーダーのパスと名前からシェーターをコンパイルする
	*rootSignatureがhlslに定義されているときは頂点シェーダーに関連付けておくこと
	*何もないときは空の引数を渡すこと
	*@brief hlslのコンパイルをしてデータを保存する
	*@param (shaderPath) シェーダのファイルパス
	*@param (vsName) バーテックスシェーダー名
	*@param (psName) ピクセルシェーダーの名
	*@param (gsName) ジオメトリシェーダー名
	*@param (hsName) ハルシェーダー名
	*@param (dsname) ドメインシェーダー名
	*@param (existRootSignature) ルートシグネチャを定義しているかどうか
	*/
	ShaderDatas CompileShader(const std::string& shaderPath,
		const std::string& vsName,
		const std::string& psName,
		const std::string& gsName,
		const std::string& hsName,
		const std::string& dsName,
		bool existRootSignature);

	ShaderDatas CompileShaderFromResource(int reosurceID,
		const std::string& vsName,
		const std::string& psName,
		const std::string& gsName,
		const std::string& hsName,
		const std::string& dsName,
		bool existRootSignature);

	void ReleaseShader(std::string shaderpath);

	void AddDefineMacro(const std::string& name, const std::string& def);

	void SetShaderModel(const std::string& shaderModel);

	void SetShaderDirPath(const std::string& shaderDirPath);

	const std::string& GetShaderDirPath() const;

private:
	ShaderCompiler();
	ShaderCompiler(const ShaderCompiler&);
	ShaderCompiler(const ShaderCompiler&&);
	ShaderCompiler& operator=(const ShaderCompiler&);

	static ShaderCompiler* mInstance;
	struct MacroData
	{
		std::string name;
		std::string def;
	};

	std::map<std::string, ShaderDatas> mDatas;
	std::vector<D3D_SHADER_MACRO> mMacros;
	std::list<MacroData> mStrData;
	std::string mShaderModel;
	std::string mShaderDirPath;
};

