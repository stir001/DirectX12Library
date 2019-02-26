#pragma once
#include<Windows.h>
#include<memory>

class Dx12Camera;

unsigned int Roundup2Multiplier(unsigned int size);

bool ProcessMessage();

void Dx12CtrlInit(HINSTANCE hInst,
	std::string wndName = "DirectX12",
	unsigned int wndWidth = 1280, unsigned int wndHeight = 720);

void Dx12CtrlEnd();

void SetShaderDir(const std::string& dirPath);

std::shared_ptr<Dx12Camera> GetCamera(unsigned int cameraIndex);