#include "stdafx.h"
#include "DrawObjectLoader.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include <wpframework.h>
#include <algorithm>

DrawObjectLoader::DrawObjectLoader()
	:mPipelinestate(nullptr), mRootsignature(nullptr), 
	mCmdList(RenderingPathManager::Instance().GetRenderingPathCommandList(0))
{
	setlocale(LC_ALL, "japanese");
}

DrawObjectLoader::~DrawObjectLoader()
{
}

void DrawObjectLoader::GetRelativePath(const std::string& path)
{
	size_t length = path.rfind('/') + 1;
	mRelativePath.resize(length);
	std::copy(path.begin(), path.begin() + length, mRelativePath.begin());
}

void DrawObjectLoader::SetRenderingCommnadList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	mCmdList = cmdList;
}