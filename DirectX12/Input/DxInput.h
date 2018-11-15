#pragma once

#include<Windows.h>


struct MousePos
{
	POINT pos;
};

//window側で定義されていない部分の
//仮想キーコードの定義と
//ゲームで使うことの多い仮想キーコードのenum化
enum eVIRTUAL_KEY_INDEX
{
	eVIRTUAL_KEY_INDEX_NON = 0x00,
	//未定義の部分を定義
	eVIRTUAL_KEY_INDEX_0 = 0x30,
	eVIRTUAL_KEY_INDEX_1 = 0x31,
	eVIRTUAL_KEY_INDEX_2 = 0x32,
	eVIRTUAL_KEY_INDEX_3 = 0x33,
	eVIRTUAL_KEY_INDEX_4 = 0x34,
	eVIRTUAL_KEY_INDEX_5 = 0x35,
	eVIRTUAL_KEY_INDEX_6 = 0x36,
	eVIRTUAL_KEY_INDEX_7 = 0x37,
	eVIRTUAL_KEY_INDEX_8 = 0x38,
	eVIRTUAL_KEY_INDEX_9 = 0x39,
	eVIRTUAL_KEY_INDEX_A = 0x41,
	eVIRTUAL_KEY_INDEX_B = 0x42,
	eVIRTUAL_KEY_INDEX_C = 0x43,
	eVIRTUAL_KEY_INDEX_D = 0x44,
	eVIRTUAL_KEY_INDEX_E = 0x45,
	eVIRTUAL_KEY_INDEX_F = 0x46,
	eVIRTUAL_KEY_INDEX_G = 0x47,
	eVIRTUAL_KEY_INDEX_H = 0x48,
	eVIRTUAL_KEY_INDEX_I = 0x49,
	eVIRTUAL_KEY_INDEX_J = 0x4A,
	eVIRTUAL_KEY_INDEX_K = 0x4B,
	eVIRTUAL_KEY_INDEX_L = 0x4C,
	eVIRTUAL_KEY_INDEX_M = 0x4D,
	eVIRTUAL_KEY_INDEX_N = 0x4E,
	eVIRTUAL_KEY_INDEX_O = 0x4F,
	eVIRTUAL_KEY_INDEX_P = 0x50,
	eVIRTUAL_KEY_INDEX_Q = 0x51,
	eVIRTUAL_KEY_INDEX_R = 0x52,
	eVIRTUAL_KEY_INDEX_S = 0x53,
	eVIRTUAL_KEY_INDEX_T = 0x54,
	eVIRTUAL_KEY_INDEX_U = 0x55,
	eVIRTUAL_KEY_INDEX_V = 0x56,
	eVIRTUAL_KEY_INDEX_W = 0x57,
	eVIRTUAL_KEY_INDEX_X = 0x58,
	eVIRTUAL_KEY_INDEX_Y = 0x59,
	eVIRTUAL_KEY_INDEX_Z = 0x5A,

	//使うことの多い部分のenum化
	eVIRTUAL_KEY_INDEX_LBUTTON = 0x01,//マウス左ボタン
	eVIRTUAL_KEY_INDEX_RBUTTON = 0x02,//マウス右ボタン
	eVIRTUAL_KEY_INDEX_MBUTTON = 0x04,//マウス中央ボタン
	eVIRTUAL_KEY_INDEX_ENTER = 0x0D,//Enter(Return)キー
	eVIRTUAL_KEY_INDEX_SHIFT = 0x10,//Shiftキー(左右を区別しない)
	eVIRTUAL_KEY_INDEX_CONTROL = 0x11,//Ctrlキー(左右を区別しない)
	eVIRTUAL_KEY_INDEX_ALT = 0x12,//Altキー(左右区別しない)
	eVIRTUAL_KEY_INDEX_LSHIFT = 0xA0,//左Shiftキー
	eVIRTUAL_KEY_INDEX_RSHIFT = 0xA1,//右Shiftキー
	eVIRTUAL_KEY_INDEX_LCONTROL = 0xA2,//左Ctrlキー
	eVIRTUAL_KEY_INDEX_RCONTROL = 0xA3,//右Ctrlキー
	eVIRTUAL_KEY_INDEX_LALT = 0xA4,//左Altキー
	eVIRTUAL_KEY_INDEX_RALT = 0xA5,//右Altキー
	eVIRTUAL_KEY_INDEX_ESCAPE = 0x1B,//ESCキー
	eVIRTUAL_KEY_INDEX_SPACE = 0x20,//Spaceキー
	eVIRTUAL_KEY_INDEX_LEFT = 0x25,//左矢印キー
	eVIRTUAL_KEY_INDEX_UP = 0x26,//上矢印キー
	eVIRTUAL_KEY_INDEX_RIGHT = 0x27,//右矢印キー
	eVIRTUAL_KEY_INDEX_DOWN = 0x28,//下矢印キー
	eVIRTUAL_KEY_INDEX_NUMPAD0 = 0x60,//ナムパッド0キー
	eVIRTUAL_KEY_INDEX_NUMPAD1 = 0x61,//ナムパッド1キー
	eVIRTUAL_KEY_INDEX_NUMPAD2 = 0x62,//ナムパッド2キー
	eVIRTUAL_KEY_INDEX_NUMPAD3 = 0x63,//ナムパッド3キー
	eVIRTUAL_KEY_INDEX_NUMPAD4 = 0x64,//ナムパッド4キー
	eVIRTUAL_KEY_INDEX_NUMPAD5 = 0x65,//ナムパッド5キー
	eVIRTUAL_KEY_INDEX_NUMPAD6 = 0x66,//ナムパッド6キー
	eVIRTUAL_KEY_INDEX_NUMPAD7 = 0x67,//ナムパッド7キー
	eVIRTUAL_KEY_INDEX_NUMPAD8 = 0x68,//ナムパッド8キー
	eVIRTUAL_KEY_INDEX_NUMPAD9 = 0x69,//ナムパッド9キー
	eVIRTUAL_KEY_INDEX_F1 = 0x70,
	eVIRTUAL_KEY_INDEX_F2 = 0x71,
	eVIRTUAL_KEY_INDEX_F3 = 0x72,
	eVIRTUAL_KEY_INDEX_F4 = 0x73,
	eVIRTUAL_KEY_INDEX_F5 = 0x74,
	eVIRTUAL_KEY_INDEX_F6 = 0x75,
	eVIRTUAL_KEY_INDEX_F7 = 0x76,
	eVIRTUAL_KEY_INDEX_F8 = 0x77,
	eVIRTUAL_KEY_INDEX_F9 = 0x78,
	eVIRTUAL_KEY_INDEX_F10 = 0x79,
	eVIRTUAL_KEY_INDEX_F11 = 0x7A,
	eVIRTUAL_KEY_INDEX_F12 = 0x7B,
};

enum eKEY_STATE_CHECK
{
	eKEY_STATE_CHECK_DOWN	= 0x80,//キーが押されているかどうかの判定
	eKEY_STATE_CHECK_TOGGLE	= 0x01,//トグルがonかoffかの判定
};

class DxInput
{
private:
	const static int mKEY_COUNT = 256;
	HWND mHWND;

	//最上位ビット(0x80)が立っているときは今押されている
	//再下位ビット(0x01)はトグルのon off判定
	//どちらもたっていないときは押されていない
	//以下のサイトを参照
	//ttp://www016.upp.so-net.ne.jp/garger-studio/gameprog/vb0142.html
	unsigned char mKeyState[mKEY_COUNT];
	unsigned char mPreKeyState[mKEY_COUNT];
	MousePos mMousePos;
	//XINPUT_STATE* xInState;
public:
	//毎フレーム呼ぶ
	bool UpdateKeyState();
	const MousePos GetMousePos();

	//@return 256の要素をもつ配列を返す
	const unsigned char* GetKeyState();

	bool IsKeyDown(eVIRTUAL_KEY_INDEX index) const;
	bool IsKeyTrigger(eVIRTUAL_KEY_INDEX index) const;
	bool IsKeyToggle(eVIRTUAL_KEY_INDEX index) const;
	DxInput();
	~DxInput();
};

