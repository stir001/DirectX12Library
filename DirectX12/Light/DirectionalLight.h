#pragma once
#include "LightObject.h"

 struct DirectionalLightElement
{
	DirectX::XMFLOAT4 tailPos;
	DirectX::XMFLOAT4X4 viewProj;
};

class DirectionalLight :
	public LightObject
{
public:
	DirectionalLight(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& tailPos, float length = 200);
	DirectionalLight(float dirX, float dirY, float dirZ, float length = 200);
	~DirectionalLight();

private:
	DirectionalLightElement mElement;
};

