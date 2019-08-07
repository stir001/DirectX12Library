#pragma once

#include <string>
#include <wrl.h>
#include <Windows.h>

class Dx12CommandList;
struct ID3D12CommandQueue;
struct ID3D12Device;
enum D3D12_COMMAND_LIST_TYPE;

class Dx12CommandQueue
{
public:
	Dx12CommandQueue(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type);
	~Dx12CommandQueue();

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Get();
	void Signal();
	void ExecuteCommandList(std::shared_ptr<Dx12CommandList>& cmdList);
	void ExecuteCommandLists(std::shared_ptr<Dx12CommandList>* cmdLists, unsigned int cmdListNum);
	void Wait();
private:
	unsigned int mFenceValue;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	HANDLE mEventHandle;
};

