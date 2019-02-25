#include "stdafx.h"
#include "DxInput.h"
#include "Master/Dx12Ctrl.h"

DxInput::DxInput()
{
	GetKeyboardState(mKeyState);
	GetCursorPos(&mMousePos.pos);
	mHWND = Dx12Ctrl::Instance().GetWndHandle();
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

const MousePos DxInput::GetMousePos()
{
	GetCursorPos(&mMousePos.pos);
	ScreenToClient(mHWND, &mMousePos.pos);
	return mMousePos;
}

bool DxInput::IsKeyDown(VIRTUAL_KEY_INDEX index)const
{
	return (mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_DOWN) != 0;
}

bool DxInput::IsKeyTrigger(VIRTUAL_KEY_INDEX index)const
{
	return ((mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_TOGGLE) ^ (mPreKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_TOGGLE)) != 0;
}

bool DxInput::IsKeyToggle(VIRTUAL_KEY_INDEX index) const
{
	return (mKeyState[static_cast<int>(index)] & eKEY_STATE_CHECK_TOGGLE) != 0;
}