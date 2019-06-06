#pragma once
#include<Windows.h>
#include<memory>
#include<string>

class Dx12Camera;
struct Dx12Camera::Property;
class FbxModelController;
class ImageController;
class Image3DController;

unsigned int Roundup2Multiplier(unsigned int size);

bool ProcessMessage();

void Dx12CtrlInit(HINSTANCE hInst,
	std::string wndName = "DirectX12",
	unsigned int wndWidth = 1280, unsigned int wndHeight = 720);

void Dx12CtrlEnd();

void SetShaderDir(const std::string& dirPath);

std::shared_ptr<Dx12Camera> GetCamera(unsigned int cameraIndex = 0U);

std::shared_ptr<Dx12Camera> AddCamera(const Dx12Camera::Property& prop, const DirectX::XMFLOAT3& eye,
	const DirectX::XMFLOAT3& target);

bool DeleteCamera(unsigned int cameraIndex);

std::shared_ptr<FbxModelController> LoadFbxMesh(const std::string& fbxPath);

std::shared_ptr<FbxModelController> LoadFMD(const std::string& fmdPath);

std::shared_ptr<ImageController> LoadImage2D(const std::string& imagePath);

std::shared_ptr<Image3DController> LoadImage3D(const std::string& imagePath);

void SetIcon(const std::string& iconName);
