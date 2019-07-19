#pragma once

#include <Windows.h>
#include <memory>
#include <Xinput.h>
#include <DirectXMath.h>

//window側で定義されていない部分の
//仮想キーコードの定義と
//ゲームで使うことの多い仮想キーコードのenum化
enum class VIRTUAL_KEY_INDEX
{
	KEY_NON = 0x00,
	//未定義の部分を定義
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,

	//使うことの多い部分のenum化
	KEY_LBUTTON = 0x01,//マウス左ボタン
	KEY_RBUTTON = 0x02,//マウス右ボタン
	KEY_MBUTTON = 0x04,//マウス中央ボタン
	KEY_ENTER = 0x0D,//Enter(Return)キー
	KEY_SHIFT = 0x10,//Shiftキー(左右を区別しない)
	KEY_CONTROL = 0x11,//Ctrlキー(左右を区別しない)
	KEY_ALT = 0x12,//Altキー(左右区別しない)
	KEY_LSHIFT = 0xA0,//左Shiftキー
	KEY_RSHIFT = 0xA1,//右Shiftキー
	KEY_LCONTROL = 0xA2,//左Ctrlキー
	KEY_RCONTROL = 0xA3,//右Ctrlキー
	KEY_LALT = 0xA4,//左Altキー
	KEY_RALT = 0xA5,//右Altキー
	KEY_ESCAPE = 0x1B,//ESCキー
	KEY_SPACE = 0x20,//Spaceキー
	KEY_LEFT = 0x25,//左矢印キー
	KEY_UP = 0x26,//上矢印キー
	KEY_RIGHT = 0x27,//右矢印キー
	KEY_DOWN = 0x28,//下矢印キー
	KEY_NUMPAD0 = 0x60,//ナムパッド0キー
	KEY_NUMPAD1 = 0x61,//ナムパッド1キー
	KEY_NUMPAD2 = 0x62,//ナムパッド2キー
	KEY_NUMPAD3 = 0x63,//ナムパッド3キー
	KEY_NUMPAD4 = 0x64,//ナムパッド4キー
	KEY_NUMPAD5 = 0x65,//ナムパッド5キー
	KEY_NUMPAD6 = 0x66,//ナムパッド6キー
	KEY_NUMPAD7 = 0x67,//ナムパッド7キー
	KEY_NUMPAD8 = 0x68,//ナムパッド8キー
	KEY_NUMPAD9 = 0x69,//ナムパッド9キー
	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,
};

enum class XINPUT_PAD_STATE
{
	DPAD_UP			=  0x00000001,
	DPAD_DOWN		=  0x00000002,
	DPAD_LEFT		=  0x00000004,
	DPAD_RIGHT		=  0x00000008,
	START			=  0x00000010,
	BACK			=  0x00000020,
	LEFT_THUMB		=  0x00000040,
	RIGHT_THUMB		=  0x00000080,
	LEFT_SHOULDER	=  0x0100,
	RIGHT_SHOULDER	=  0x0200,
	A				=  0x1000,
	B				=  0x2000,
	X				=  0x4000,
	Y				=  0x8000,

};

enum eKEY_STATE_CHECK
{
	eKEY_STATE_CHECK_DOWN	= 0x80,//キーが押されているかどうかの判定
	eKEY_STATE_CHECK_TOGGLE	= 0x01,//トグルがonかoffかの判定
};

class DxInput
{
public:
	DxInput();
	~DxInput();
	//毎フレーム呼ぶ
	bool UpdateKeyState();
	const DirectX::XMFLOAT2 GetMousePos() const;

	//@return 256の要素をもつ配列を返す
	const unsigned char* GetKeyState();

	bool IsKeyDown(VIRTUAL_KEY_INDEX index) const;
	bool IsKeyTrigger(VIRTUAL_KEY_INDEX index) const;
	bool IsKeyToggle(VIRTUAL_KEY_INDEX index) const;

	bool IsKeyUp(VIRTUAL_KEY_INDEX index) const;
private:
	const static int KEY_COUNT = 256;
	const static int PAD_NUM = 4;
	HWND mHWND;

	//最上位ビット(0x80)が立っているときは今押されている
	//再下位ビット(0x01)はトグルのon off判定
	//どちらもたっていないときは押されていない
	//以下のサイトを参照
	//ttp://www016.upp.so-net.ne.jp/garger-studio/gameprog/vb0142.html
	unsigned char mKeyState[KEY_COUNT];
	unsigned char mPreKeyState[KEY_COUNT];

	std::vector<XINPUT_STATE> mXInputState;
};

