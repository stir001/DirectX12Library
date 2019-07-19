#include "stdafx.h"
#include "DxInput.h"
#include "Master/Dx12Ctrl.h"

DxInput::DxInput()
{
	GetKeyboardState(mKeyState);
	mHWND = Dx12Ctrl::Instance().GetWindowHandle();
	mXInputState.resize(PAD_NUM);
}

DxInput::~DxInput()
{
}

bool DxInput::UpdateKeyState()
{
	memcpy(mPreKeyState, mKeyState, sizeof(mKeyState));
	for (unsigned int i = 0; i < PAD_NUM; ++i)
	{
		XInputGetState(i, &mXInputState[i]);
	}

	return GetKeyboardState(mKeyState) != 0;
}

const unsigned char* DxInput::GetKeyState()
{
	return mKeyState;
}

const DirectX::XMFLOAT2 DxInput::GetMousePos() const
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(mHWND, &mouse);
	return { static_cast<float>(mouse.x), static_cast<float>(mouse.y) };
}

bool DxInput::IsKeyDown(VIRTUAL_KEY_INDEX index) const
{
	return (mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_DOWN) != 0;
}

bool DxInput::IsKeyTrigger(VIRTUAL_KEY_INDEX index) const
{
	return ((mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_TOGGLE) ^ (mPreKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_TOGGLE)) != 0;
}

bool DxInput::IsKeyToggle(VIRTUAL_KEY_INDEX index) const
{
	return (mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_TOGGLE) != 0;
}

bool DxInput::IsKeyUp(VIRTUAL_KEY_INDEX index) const
{
	return (!(mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_DOWN)) && (mPreKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_DOWN);
}
