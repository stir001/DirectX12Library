#include "stdafx.h"
#include "DirectionalLight.h"
#include "Util/XMFloatOperators.h"
#include "Buffer/ConstantBufferObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Master/Dx12Ctrl.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(DirectX::XMFLOAT3 & pos, DirectX::XMFLOAT3 & dir, float length)
{
	mPos = pos;
	mLength = length;
	mElement.dir = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(dir));

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { mElement.dir.x * length, mElement.dir.y * length, mElement.dir.z * length };

	DirectX::XMFLOAT3 t_dir = { mElement.dir.x, mElement.dir.y, mElement.dir.z };
	mElement.dir = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(t_dir));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&pos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(50, 50, 1.0f, 200.0f);//lightprojectionçsóÒ
	XMStoreFloat4x4(&mElement.viewProj, lightview * lightprojection);
	mCbuffer = std::make_shared<ConstantBufferObject>("DirectionalLightConstantBuffer",Dx12Ctrl::Instance().GetDev(),static_cast<unsigned int>(sizeof(DirectionalLightElement)), 1U);

	mCbuffer->WriteBuffer(&mElement, sizeof(mElement));
}

DirectionalLight::DirectionalLight(float dirX, float dirY, float dirZ, float length)
{
	mPos = {0, 0, 0};
	mLength = length;
	DirectX::XMFLOAT3 tdir = { dirX, dirY, dirZ };
	mPos = tdir * length * 0.5f;
	mElement.dir = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(tdir));

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { mElement.dir.x * length, mElement.dir.y * length, mElement.dir.z * length };

	DirectX::XMFLOAT3 t_dir = { mElement.dir.x, mElement.dir.y, mElement.dir.z };
	mElement.dir = ConvertXMFloat3ToXMFloat4(NormalizeXMFloat3(t_dir));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(50, 50, 1.0f, length);//lightprojectionçsóÒ
	XMStoreFloat4x4(&mElement.viewProj, lightview * lightprojection);
	mCbuffer = std::make_shared<ConstantBufferObject>("DirectionalLightConstantBuffer", Dx12Ctrl::Instance().GetDev(), static_cast<unsigned int>(sizeof(DirectionalLightElement)), 1);

	mCbuffer->WriteBuffer(&mElement, sizeof(mElement));
}

DirectionalLight::~DirectionalLight()
{
}
