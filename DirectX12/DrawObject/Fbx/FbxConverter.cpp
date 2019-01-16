#include "stdafx.h"
#include "FbxConverter.h"
#include "Dx12MSLib.h"
#include "DirectX12/PipelineState/FbxPipelineState.h"
#include "DirectX12/Rootsignature/FbxRootSignature.h"
#include "FMDdata.h"
#include "FMDLoader.h"

FbxConverter* FbxConverter::mInstance = nullptr;

FbxConverter::FbxConverter() 
	: mIsConverting(false)
	, mConvertModel(nullptr)
	, mDataConverter(std::make_shared<FbxModelDataConverter>())
	, mWriteFilePath("FMD/")
	, mFmdLoader(std::make_shared<FMDLoader>())
{
}

FbxConverter::~FbxConverter()
{
}

void FbxConverter::Initialize()
{
	auto device = Dx12Ctrl::Instance().GetDev();

	mRootsignature = std::make_shared<FbxRootSignature>(device);
	mPipelineState = std::make_shared<FbxPipelineState>(mRootsignature, device);
	mWorldLight = std::make_shared<DirectionalLight>(0.5f, -1.0f, 1.0f);
}

void FbxConverter::ConvertFile()
{
	if (mIsConverting == false && mFilePath.size() != 0)
	{
		FbxLoader::Instance().ReleaseAllModel();
		mConvertModel = nullptr;
		mIsConverting = true;
		(this->*mConvertFunc)();
		mIsConverting = false;
		mFilePath.clear();
	}
}

void FbxConverter::ConvertFmd()
{
	auto fmd = mFmdLoader->LoadFMD(mFilePath);
	CreateController(fmd);
}

void FbxConverter::SetLoadFilePath(const std::string& path)
{
	if (mIsConverting == true)
	{
		return;
	}
	mFilePath.clear();
	auto little =  path.rfind(".fbx");
	auto big = path.rfind(".FBX");
	auto fmd = path.rfind(".fmd");
	auto index = little < big ? little : big;
	if (index != 0 && index != std::string::npos)
	{
		mFilePath = path;
		mConvertFunc = &FbxConverter::ConvertFbx;
	}
	else if (fmd != std::string::npos)
	{
		mFilePath = path;
		mConvertFunc = &FbxConverter::ConvertFmd;
	}
	for (auto& c : mFilePath)
	{
		if (c == '\\')
		{
			c = '/';
		}
	}
}

void FbxConverter::Draw()
{
	if (mConvertModel != nullptr)
	{
		mConvertModel->Draw();
		mConvertModel->DrawSkeleton();
	}
}

void FbxConverter::ConvertFbx()
{
	auto modelData = FbxLoader::Instance().GetMeshData(mFilePath);

	if (modelData == nullptr)
	{
		mIsConverting = false;
		mFilePath.clear();
		return;
	}

	WriteFMDFile(modelData);

	//GetConvertFileName(modelData->modelPath);

	CreateController(modelData);
}

void FbxConverter::CreateController(std::shared_ptr<Fbx::FbxModelData> modelData)
{
	auto model = mDataConverter->ConvertToFbxModel(modelData);
	auto cmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model));
	auto device = Dx12Ctrl::Instance().GetDev();
	mConvertModel = std::make_shared<FbxModelController>(model, device, cmdList, mPipelineState, mRootsignature);
	mConvertModel->SetLight(mWorldLight);
}

void FbxConverter::CreateController(FMDFileData & data)
{
	auto model = mDataConverter->ConvertToFbxModel(data);
	auto cmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model));
	auto device = Dx12Ctrl::Instance().GetDev();
	mConvertModel = std::make_shared<FbxModelController>(model, device, cmdList, mPipelineState, mRootsignature);
	mConvertModel->SetLight(mWorldLight);
}

std::string FbxConverter::GetConvertFileName(const std::string & modelPath) const
{
	auto slashFind = modelPath.rfind('/') % std::string::npos;
	auto yenFind = modelPath.rfind('\\') % std::string::npos;
	auto findpos = (slashFind < yenFind ? yenFind : slashFind);
	if (findpos == std::string::npos)
	{
		findpos = 0;
	}
	else
	{
		++findpos;
	}
	std::string convertFileName(modelPath.begin() + findpos, modelPath.end());
	auto finddot = convertFileName.rfind('.');
	convertFileName.erase(convertFileName.begin() + finddot, convertFileName.end());
	convertFileName += ".fmd";
	return  convertFileName;
}

void FbxConverter::WriteFMDFile(const std::shared_ptr<Fbx::FbxModelData>& modelData)
{
	char FMD_FILE_HEADER[4] = "FMD";

	auto filePath = CreateWriteFilePath(modelData->modelPath);
	
	std::ofstream filestream(filePath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	if (!filestream)
	{
		filestream.close();//デストラクタで呼ばれるから呼ばなくてもいいっぽい
		return; //ファイルオープン失敗
	}
	filestream.write(FMD_FILE_HEADER, 4);

	WriteVertices(filestream, modelData);
	WriteIndices(filestream, modelData);
	WriteMaterials(filestream, modelData);
	WriteSkeleton(filestream, modelData);

	filestream.close();
}

std::string FbxConverter::CreateWriteFilePath(const std::string & modelPath) const
{
	auto slashpos = modelPath.rfind('/');
	if (slashpos == std::string::npos)
	{
		slashpos = 0;
	}
	else
	{
		++slashpos;
	}

	auto dotpos = modelPath.rfind('.');
	std::string writeFilePath(modelPath.begin() + slashpos, modelPath.begin() + dotpos);
	writeFilePath = mWriteFilePath + writeFilePath + ".fmd";
	return writeFilePath;
}

void FbxConverter::WriteVertices(std::ofstream & stream, const std::shared_ptr<Fbx::FbxModelData>& modelData)
{
	auto& vertices = modelData->vertexesInfo.vertexes;
	int vertexNum = static_cast<int>(vertices.size());
	stream.write(reinterpret_cast<const char*>(&vertexNum), sizeof(vertexNum));
	
	int vertexPNTsize = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT2);

	for (int i = 0; i < vertexNum; ++i)
	{
		FMDVertexEffectBone bone;
		stream.write(reinterpret_cast<const char*>(&vertices[i]), vertexPNTsize);
		int boneNum = static_cast<int>(vertices[i].boneIndex.size());
		stream.write(reinterpret_cast<const char*>(&boneNum), sizeof(boneNum));
		for (int j = 0; j < boneNum; ++j)
		{
			bone.boneIndex = vertices[i].boneIndex[j];
			bone.boneWeight = vertices[i].boneWeight[j];
			stream.write(reinterpret_cast<const char*>(&bone), sizeof(bone));
		}
	}
}

void FbxConverter::WriteIndices(std::ofstream & stream, const std::shared_ptr<Fbx::FbxModelData>& modelData)
{
	int indexNum = static_cast<int>(modelData->indexes.indexes.size());
	stream.write(reinterpret_cast<const char*>(&indexNum), sizeof(indexNum));
	stream.write(reinterpret_cast<const char*>(modelData->indexes.indexes.data()), sizeof(modelData->indexes.indexes[0]) * indexNum);
}

void FbxConverter::WriteMaterials(std::ofstream & stream, const std::shared_ptr<Fbx::FbxModelData>& modelData)
{
	auto& materials = modelData->materials;
	int materialNum = static_cast<int>(materials.size());
	stream.write(reinterpret_cast<const char*>(&materialNum), sizeof(materialNum));

	FMDMaterials mats(materialNum);
	FMDTextures tex(materialNum);
	for (int i = 0; i < materialNum; ++i)
	{
		mats[i] = materials[i];
		tex[i] = materials[i];
	}

	stream.write(reinterpret_cast<const char*>(mats.data()), sizeof(mats[0]) * materialNum);

	for (int i = 0; i < materialNum; ++i)
	{
		auto& t = tex[i];
		stream.write(reinterpret_cast<const char*>(t.pathSizeTable)
			, sizeof(t.pathSizeTable[0]) * static_cast<int>(FMDTexture::FMDTextureTable::tableMax));
		auto alldata = t.GetAllData();
		stream.write(reinterpret_cast<const char*>(alldata.data()), sizeof(char) * alldata.size());
	}
}

void FbxConverter::WriteSkeleton(std::ofstream & stream, const std::shared_ptr<Fbx::FbxModelData>& modelData)
{
	auto& skeletons = modelData->skeletons;
	auto& sklIndices = modelData->skeletonIndices;
	int skeletonNum = static_cast<int>(skeletons.size());
	stream.write(reinterpret_cast<const char*>(&skeletonNum), sizeof(skeletonNum));
	FMDSkeleton s;
	int dataSize = sizeof(s.pos) + sizeof(s.rotation) + sizeof(s.scale) + sizeof(s.initMatrix) +sizeof(s.parentIndex);
	auto writeFunc = [&stream, dataSize](FMDSkeleton& s, const Fbx::FbxSkeleton& skl)
	{
		s = skl;
		stream.write(reinterpret_cast<const char*>(&s), dataSize);
		s.nameSize = static_cast<int>(s.name.size());
		stream.write(reinterpret_cast<const char*>(&s.nameSize), sizeof(s.nameSize));
		stream.write(reinterpret_cast<const char*>(s.name.data()), sizeof(char) * s.nameSize);
	};

	for (int i = 0; i < static_cast<int>(skeletons.size()); ++i)
	{
		writeFunc(s, skeletons[i]);
	}
}