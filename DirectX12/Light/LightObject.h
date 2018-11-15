#pragma once

#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

struct ID3D12GraphicsCommandList;
class ConstantBufferObject;

class LightObject
{
public:
	LightObject();
	virtual ~LightObject();

	std::shared_ptr<ConstantBufferObject>& GetLightBuffer();
protected:
	DirectX::XMFLOAT3 mPos;//ç¿ïW
	float mLength;//ãóó£å¿äE
	std::shared_ptr<ConstantBufferObject> mCbuffer;
};

