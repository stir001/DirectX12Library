#include "stdafx.h"
#include "DirectionalLight.h"
#include "Util/XMFloatOperators.h"
#include "Buffer/ConstantBufferObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Master/Dx12Ctrl.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(DirectX::XMFLOAT3 & pos, DirectX::XMFLOAT3 & tailPos, float length)
{
	mPos = pos;
	mLength = length;
	mElement.tailPos = ConvertToXMFloat4(Normalize(tailPos));

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = { mElement.tailPos.x * length, mElement.tailPos.y * length, mElement.tailPos.z * length };

	DirectX::XMFLOAT3 t_dir = { mElement.tailPos.x, mElement.tailPos.y, mElement.tailPos.z };
	mElement.tailPos = ConvertToXMFloat4(Normalize(t_dir));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&pos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(70, 70, 1.0f, length);//lightprojectionçsóÒ
	XMStoreFloat4x4(&mElement.viewProj, lightview * lightprojection);
	mCbuffer = std::make_shared<ConstantBufferObject>("DirectionalLightConstantBuffer",Dx12Ctrl::Instance().GetDev(),static_cast<unsigned int>(sizeof(DirectionalLightElement)), 1U);

	mCbuffer->WriteBuffer(&mElement, sizeof(mElement));
}

DirectionalLight::DirectionalLight(float dirX, float dirY, float dirZ, float length)
{
	mPos = {0, 0, 0};
	mLength = length;
	DirectX::XMFLOAT3 tdir = { dirX, dirY, dirZ };
	mElement.tailPos = ConvertToXMFloat4(Normalize(tdir));
	mPos = { -mElement.tailPos.x * length * 0.5f,- mElement.tailPos.y * length * 0.5f, -mElement.tailPos.z * length * 0.5f};

	XMVECTOR upper = { 0,1,0 };
	XMVECTOR target = {0, 0,0 };

	DirectX::XMFLOAT3 t_dir = { mElement.tailPos.x, mElement.tailPos.y, mElement.tailPos.z };
	mElement.tailPos = ConvertToXMFloat4(Normalize(t_dir));
	DirectX::XMVECTOR lightpos = XMLoadFloat3(&mPos);
	DirectX::XMMATRIX lightview = DirectX::XMMatrixLookAtLH(lightpos, target, upper);
	DirectX::XMMATRIX lightprojection = DirectX::XMMatrixOrthographicLH(100, 100, 1.0f, length);//lightprojectionçsóÒ
	XMStoreFloat4x4(&mElement.viewProj, lightview * lightprojection);
	mCbuffer = std::make_shared<ConstantBufferObject>("DirectionalLightConstantBuffer", Dx12Ctrl::Instance().GetDev(), static_cast<unsigned int>(sizeof(DirectionalLightElement)), 1);

	mCbuffer->WriteBuffer(&mElement, sizeof(mElement));
}

DirectionalLight::~DirectionalLight()
{
}
