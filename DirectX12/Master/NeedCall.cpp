#include "stdafx.h"
#include "Dx12Ctrl.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "Animation/AnimationPlayerUpdater.h"
#include "Shader/ShaderCompiler.h"
#include "Camera/CameraHolder.h"
#include "Camera/Dx12Camera.h"
#include "NeedCall.h"

unsigned int Roundup2Multiplier(unsigned int size)
{
	unsigned int bit = 0x8000000;//32bit‚ÌŽž
	for (unsigned int i = 31; i >= 0; i--)
	{
		if (size & bit)break;
		bit >>= 1;
	}

	return size + ((bit << 1) - size) % bit;
}

void CallEveryFrame()
{
	AnimationPlayerUpdater::Instance().WaitAnimation();
	RenderingPassManager::Instance().Render();
	AnimationPlayerUpdater::Instance().UpdateAnimations();
}

bool ProcessMessage()
{
	CallEveryFrame();

	MSG msg = {};
	bool rtn = true;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		rtn = false;
	}

	return rtn;
}

void Dx12CtrlInit(HINSTANCE hInst,
	std::string wndName,
	unsigned int wndWidth, unsigned int wndHeight)
{
	Dx12Ctrl::Instance().SetWindowSize(wndWidth, wndHeight);
	Dx12Ctrl::Instance().SetWindowName(wndName);
	Dx12Ctrl::Instance().Dx12Init(hInst);
}

void Dx12CtrlEnd()
{
	Dx12Ctrl::Instance().Release();
	Dx12Ctrl::Destroy();
}

void SetShaderDir(const std::string& dirPath)
{
	ShaderCompiler::Instance().SetShaderDirPath(dirPath);
}

std::shared_ptr<Dx12Camera> GetCamera(unsigned int cameraIndex)
{
	return Dx12Ctrl::Instance().GetCameraHolder()->GetCamera(cameraIndex);
}

std::shared_ptr<Dx12Camera> AddCamera(const Dx12Camera::Property& prop, const DirectX::XMFLOAT3& eye,
	const DirectX::XMFLOAT3& target)
{
	D3D12_VIEWPORT port = {
		prop.viewTopLeftX,
		prop.viewTopLeftY,
		prop.viewWidth,
		prop.viewHeight,
		prop.viewMinDepth,
		prop.viewMaxDepth,
	};

	D3D12_RECT rect
	{
		prop.rectLeft,
		prop.rectTop,
		prop.rectRight,
		prop.rectBottom
	};
	return Dx12Ctrl::Instance().GetCameraHolder()->AddCamera(eye, target, port, rect);
}

bool DeleteCamera(unsigned int cameraIndex)
{
	return Dx12Ctrl::Instance().GetCameraHolder()->DeleteCamera(cameraIndex);
}
