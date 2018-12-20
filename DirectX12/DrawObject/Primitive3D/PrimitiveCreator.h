#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <memory>

class LightObject;
class PrimitiveController;
class RootSignatureObject;
class PipelineStateObject;
class PrimitiveObject;

class PrimitiveCreator
{
public:
	~PrimitiveCreator();

	static PrimitiveCreator& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new PrimitiveCreator();
		}
		return *mInstance;
	};

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	void SetLightObject(std::shared_ptr<LightObject> inlight);
	std::shared_ptr<PrimitiveController> CreatePlane(const DirectX::XMFLOAT3& pos, float length, float height, const DirectX::XMFLOAT3& normal);
	std::shared_ptr<PrimitiveController> CreateCube(float length, const std::string& texPath = "");
	std::shared_ptr<PrimitiveController> CreateCubeNormalMap(float length, const std::string& texPath);
	std::shared_ptr<PrimitiveController> CreateSphere(float radius, unsigned int div, const std::string& texPath = "");
	void SetParamaters(std::shared_ptr<PrimitiveController>& ctrl);
	void SetRenderingCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
private:
	PrimitiveCreator();

	static PrimitiveCreator* mInstance;
	PrimitiveCreator(PrimitiveCreator&);
	PrimitiveCreator(PrimitiveCreator&&);
	PrimitiveCreator& operator=(const PrimitiveController&);

	std::shared_ptr<LightObject> mLight;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommnadList;
	std::shared_ptr<RootSignatureObject> mRootsiganture;
	std::shared_ptr<PipelineStateObject> mPipelineState;
	std::shared_ptr<RootSignatureObject> mNormalMapRootsignature;
	std::shared_ptr<PipelineStateObject> mNormalMapPipelineState;

	std::shared_ptr<PrimitiveController> CreateController(const std::shared_ptr<PrimitiveObject>& primitive);
	void SetTexture(std::shared_ptr<PrimitiveController>& ctrl, const std::string& texPath);
};

