#pragma once
#include "ShaderDatasStructure.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <memory>

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
	std::shared_ptr<ShaderDatas> CompileShader(const std::string& shaderPath,
		const std::string& vsName,
		const std::string& psName,
		const std::string& gsName,
		const std::string& hsName,
		const std::string& dsName,
		bool existRootSignature);

	/** TODO:リソース化したシェーダーから読み込む
	*	@brief	未実装
	*/
	std::shared_ptr<ShaderDatas> CompileShaderFromResource(
		const int vsID,
		const int psID,
		const int gsID,
		const int hsID,
		const int dsID,
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

	struct ShaderNames {
		std::string vs;
		std::string ps;
		std::string gs;
		std::string hs;
		std::string ds;
		bool operator==(const ShaderNames& val) const;
	};

	Microsoft::WRL::ComPtr<ID3DBlob> GetCompiledShader(const std::wstring& filePath,
		ID3DInclude& includer, const std::string& name, const std::string& model, unsigned int compileFlag);

	ShaderResource GetShaderResource(int id);

	Microsoft::WRL::ComPtr<ID3DBlob> GetRootSignature(const void* ptr, size_t size);

	//bool 

	std::map<std::string, std::list<std::pair<ShaderNames,std::shared_ptr<ShaderDatas>>>> mDatas;
	std::vector<D3D_SHADER_MACRO> mMacros;
	std::list<MacroData> mStrData;
	std::string mShaderModel;
	std::string mShaderDirPath;
};

