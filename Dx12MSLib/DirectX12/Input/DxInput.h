#pragma once

#include<Windows.h>


struct MousePos
{
	POINT pos;
};

//window���Œ�`����Ă��Ȃ�������
//���z�L�[�R�[�h�̒�`��
//�Q�[���Ŏg�����Ƃ̑������z�L�[�R�[�h��enum��
enum eVIRTUAL_KEY_INDEX
{
	eVIRTUAL_KEY_INDEX_NON = 0x00,
	//����`�̕������`
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

	//�g�����Ƃ̑���������enum��
	eVIRTUAL_KEY_INDEX_LBUTTON = 0x01,//�}�E�X���{�^��
	eVIRTUAL_KEY_INDEX_RBUTTON = 0x02,//�}�E�X�E�{�^��
	eVIRTUAL_KEY_INDEX_MBUTTON = 0x04,//�}�E�X�����{�^��
	eVIRTUAL_KEY_INDEX_ENTER = 0x0D,//Enter(Return)�L�[
	eVIRTUAL_KEY_INDEX_SHIFT = 0x10,//Shift�L�[(���E����ʂ��Ȃ�)
	eVIRTUAL_KEY_INDEX_CONTROL = 0x11,//Ctrl�L�[(���E����ʂ��Ȃ�)
	eVIRTUAL_KEY_INDEX_ALT = 0x12,//Alt�L�[(���E��ʂ��Ȃ�)
	eVIRTUAL_KEY_INDEX_LSHIFT = 0xA0,//��Shift�L�[
	eVIRTUAL_KEY_INDEX_RSHIFT = 0xA1,//�EShift�L�[
	eVIRTUAL_KEY_INDEX_LCONTROL = 0xA2,//��Ctrl�L�[
	eVIRTUAL_KEY_INDEX_RCONTROL = 0xA3,//�ECtrl�L�[
	eVIRTUAL_KEY_INDEX_LALT = 0xA4,//��Alt�L�[
	eVIRTUAL_KEY_INDEX_RALT = 0xA5,//�EAlt�L�[
	eVIRTUAL_KEY_INDEX_ESCAPE = 0x1B,//ESC�L�[
	eVIRTUAL_KEY_INDEX_SPACE = 0x20,//Space�L�[
	eVIRTUAL_KEY_INDEX_LEFT = 0x25,//�����L�[
	eVIRTUAL_KEY_INDEX_UP = 0x26,//����L�[
	eVIRTUAL_KEY_INDEX_RIGHT = 0x27,//�E���L�[
	eVIRTUAL_KEY_INDEX_DOWN = 0x28,//�����L�[
	eVIRTUAL_KEY_INDEX_NUMPAD0 = 0x60,//�i���p�b�h0�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD1 = 0x61,//�i���p�b�h1�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD2 = 0x62,//�i���p�b�h2�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD3 = 0x63,//�i���p�b�h3�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD4 = 0x64,//�i���p�b�h4�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD5 = 0x65,//�i���p�b�h5�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD6 = 0x66,//�i���p�b�h6�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD7 = 0x67,//�i���p�b�h7�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD8 = 0x68,//�i���p�b�h8�L�[
	eVIRTUAL_KEY_INDEX_NUMPAD9 = 0x69,//�i���p�b�h9�L�[
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
	eKEY_STATE_CHECK_DOWN	= 0x80,//�L�[��������Ă��邩�ǂ����̔���
	eKEY_STATE_CHECK_TOGGLE	= 0x01,//�g�O����on��off���̔���
};

class DxInput
{
private:
	const static int mKEY_COUNT = 256;
	HWND mHWND;

	//�ŏ�ʃr�b�g(0x80)�������Ă���Ƃ��͍�������Ă���
	//�ĉ��ʃr�b�g(0x01)�̓g�O����on off����
	//�ǂ���������Ă��Ȃ��Ƃ��͉�����Ă��Ȃ�
	//�ȉ��̃T�C�g���Q��
	//ttp://www016.upp.so-net.ne.jp/garger-studio/gameprog/vb0142.html
	unsigned char mKeyState[mKEY_COUNT];
	unsigned char mPreKeyState[mKEY_COUNT];
	MousePos mMousePos;
	//XINPUT_STATE* xInState;
public:
	//���t���[���Ă�
	bool UpdateKeyState();
	const MousePos GetMousePos();

	//@return 256�̗v�f�����z���Ԃ�
	const unsigned char* GetKeyState();

	bool IsKeyDown(eVIRTUAL_KEY_INDEX index) const;
	bool IsKeyTrigger(eVIRTUAL_KEY_INDEX index) const;
	bool IsKeyToggle(eVIRTUAL_KEY_INDEX index) const;
	DxInput();
	~DxInput();
};

