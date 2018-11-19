#include "stdafx.h"
#include "Dx12Ctrl.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "Animation/AnimationPlayerManager.h"

unsigned int Roundup2Multiplier(unsigned int size)
{
	unsigned int bit = 0x8000000;//32bit‚Ì
	for (unsigned int i = 31; i >= 0; i--)
	{
		if (size & bit)break;
		bit >>= 1;
	}

	return size + ((bit << 1) - size) % bit;
}

void CallEveryFrame()
{
	AnimationPlayerManager::Instance().WaitSafeFree();
	RenderingPassManager::Instance().Render();
	AnimationPlayerManager::Instance().UpdateAnimations();
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