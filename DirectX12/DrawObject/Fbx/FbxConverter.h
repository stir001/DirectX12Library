#pragma once
#include <memory>
#include <thread>
#include <fstream>

class FbxModelController;
class FbxModelDataConverter;
class LightObject;
class PipelineStateObject;
class RootSignatureObject;
class FMDLoader;
struct FMDFileData;

namespace Fbx
{
	struct FbxModelData;
}

class FbxConverter
{
public:
	~FbxConverter();

	static FbxConverter& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new FbxConverter;
		}

		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	void Initialize();

	void ConvertFile();

	void SetLoadFilePath(const std::string& path);

	void Draw();
private:
	FbxConverter();
	void ConvertFbx();
	void ConvertFmd();
	void CreateController(std::shared_ptr<Fbx::FbxModelData> modelData);
	void CreateController(FMDFileData& data);
	std::string GetConvertFileName(const std::string& modelPath) const;
	void WriteFMDFile(const std::shared_ptr<Fbx::FbxModelData>& modelData);
	std::string CreateWriteFilePath(const std::string& modelPath) const;
	void WriteVertices(std::ofstream& stream, const std::shared_ptr<Fbx::FbxModelData>& modelData);
	void WriteIndices(std::ofstream& stream, const std::shared_ptr<Fbx::FbxModelData>& modelData);
	void WriteMaterials(std::ofstream& stream, const std::shared_ptr<Fbx::FbxModelData>& modelData);
	void WriteSkeleton(std::ofstream& stream, const std::shared_ptr<Fbx::FbxModelData>& modelData);

	static FbxConverter* mInstance;

	std::shared_ptr<FbxModelController> mConvertModel;
	std::shared_ptr<FbxModelDataConverter> mDataConverter;
	std::shared_ptr<LightObject> mWorldLight;
	std::shared_ptr<PipelineStateObject> mPipelineState;
	std::shared_ptr<RootSignatureObject> mRootsignature;
	std::string mFilePath;
	bool mIsConverting;
	std::string mWriteFilePath;
	std::shared_ptr<FMDLoader> mFmdLoader;
	void (FbxConverter::*mConvertFunc)();
};

