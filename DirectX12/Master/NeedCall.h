#pragma once
#include<Windows.h>

unsigned int Roundup2Multiplier(unsigned int size);

bool ProcessMessage();

void Dx12CtrlInit(HINSTANCE hInst,
	std::string wndName = "DirectX12",
	unsigned int wndWidth = 1280, unsigned int wndHeight = 720);