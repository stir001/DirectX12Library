#pragma once
#include <DirectXMath.h>

/**
*	@ingroup Dx12Camera
*	@struct Dx12CameraBuffer
*	@brief GPUに投げるための要素をまとめた構造体
*/
struct Dx12CameraBuffer
{
	DirectX::XMFLOAT4 eye;
	DirectX::XMFLOAT4 target;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	Dx12CameraBuffer(): eye{0.0f, 0.0f, 0.0f, 1.0f},
		target{0.0f, 0.0f, 1.0f, 1.0f},
		world{ 1.0f,0.0f,0.0f,0.0f,
			   0.0f,1.0f,0.0f,0.0f,
			   0.0f,0.0f,1.0f,0.0f,
			   0.0f,0.0f,0.0f,1.0f},
		view{ 1.0f,0.0f,0.0f,0.0f,
			  0.0f,1.0f,0.0f,0.0f,
			  0.0f,0.0f,1.0f,0.0f,
			  0.0f,0.0f,0.0f,1.0f},
		projection{ 1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					0.0f,0.0f,0.0f,1.0f}
	{}
};