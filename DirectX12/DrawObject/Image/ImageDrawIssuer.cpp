#include "stdafx.h"
#include "ImageDrawIssuer.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"

ImageDrawIssuer::ImageDrawIssuer(const ImageDrawIssuer::ConstructorArg& arg)
	: mDescHeap(arg.descHeap), mPipelinestate(arg.pipelineState)
	, mRootsignature(arg.rootSignature), mTexViewIndex(arg.texViewIndex)
{
	mVertexBuffers.resize(VERTEXIDR_MAX);
	mVertexBuffers[VERTEX] = arg.vertexBuffer;
	mVertexBuffers[UV] = arg.uvBuffer;
	mVertexBuffers[MATRIX] = arg.matrixBuffer;
	mBundleCmdList = std::make_shared<Dx12CommandList>(arg.name, arg.dev, D3D12_COMMAND_LIST_TYPE_BUNDLE);
	mBundleCmdList->Close();
}

ImageDrawIssuer::~ImageDrawIssuer()
{
}

void ImageDrawIssuer::IssueDrawCall(std::shared_ptr<Dx12CommandList>& cmdList)
{
	cmdList->SetDescriptorHeap(mDescHeap);
	cmdList->IASetVertexBuffers(mVertexBuffers.data(), static_cast<unsigned int>(mVertexBuffers.size()));
	cmdList->ExecuteBundle(mBundleCmdList);
	cmdList->DrawInstanced(4, GetInstanceNum(), 0, 0);
	ResetInstanceNum();
}

void ImageDrawIssuer::UpdateBundle()
{
	mBundleCmdList->Reset();
	auto bundle = mBundleCmdList;
	mDescHeap->SetDescriptorHeap(bundle);
	bundle->SetPipelineState(mPipelinestate);
	bundle->SetGraphicsRootSignature(mRootsignature);
	bundle->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	mDescHeap->SetGraphicsDescriptorTable(bundle, mTexViewIndex, eROOT_PARAMATER_INDEX_TEXTURE);

	bundle->Close();
}

void ImageDrawIssuer::SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mPipelinestate = pipelineState;
}

void ImageDrawIssuer::SetRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature)
{
	mRootsignature = rootSignature;
}

void ImageDrawIssuer::SetVertexBuffer(const std::shared_ptr<VertexBufferObject>& vertex)
{
	mVertexBuffers[VERTEX] = vertex;
}

void ImageDrawIssuer::SetUVBuffer(const std::shared_ptr<VertexBufferObject>& uv)
{
	mVertexBuffers[UV] = uv;
}

void ImageDrawIssuer::SetMatrixBuffer(const std::shared_ptr<VertexBufferObject>& mat)
{
	mVertexBuffers[MATRIX] = mat;
}
