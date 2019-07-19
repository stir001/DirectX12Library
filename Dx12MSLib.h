#pragma once

/**
*	Animation�t�H���_�C���N���[�h
*/
#include "DirectX12/Animation/Animation.h"
#include "DirectX12/Animation/AnimationPlayer.h"
#include "DirectX12/Animation/AnimationPlayerUpdater.h"
#include "DirectX12/Animation/VMD/VMDLoader.h"
#include "DirectX12/Animation/VMD/VMDAnimation.h"
#include "DirectX12/Animation/VMD/VMDPlayer.h"

/**
*	Buffer�t�H���_�C���N���[�h
*/
#include "DirectX12/Buffer/Dx12BufferObject.h"
#include "DirectX12/Buffer/ConstantBufferObject.h"
#include "DirectX12/Buffer/AdditionalConstatntBuffer.h"
#include "DirectX12/Buffer/DepthBufferObject.h"
#include "DirectX12/Buffer/IndexBufferObject.h"
#include "DirectX12/Buffer/RendertargetObject.h"
#include "DirectX12/Buffer/ShaderResourceObject.h"
#include "DirectX12/Buffer/VertexBufferObject.h"

#include "DirectX12/Camera/Dx12Camera.h"
#include "DirectX12/Camera/CameraHolder.h"

#include "DirectX12/CommandList/Dx12CommandList.h"

#include "DirectX12/DescriptorHeap/Dx12DescriptorHeapObject.h"

/**
*	DrawObject�t�H���_�C���N���[�h
*/
#include "DirectX12/DrawObject/DrawController3D.h"
#include "DirectX12/DrawObject/DrawObjectController.h"
#include "DirectX12/DrawObject/DrawObjectLoader.h"
#include "DirectX12/DrawObject/Model.h"
#include "DirectX12/DrawObject/Transform3DCalculator.h"
#include "DirectX12/DrawObject/TransformMatrixCalculator.h"
#include "DirectX12/DrawObject/Fbx/FbxLoader.h"
#include "DirectX12/DrawObject/Fbx/FbxModel.h"
#include "DirectX12/DrawObject/Fbx/FbxModelController.h"
#include "DirectX12/DrawObject/Fbx/FbxModelDataCoverter.h"
#include "DirectX12/DrawObject/Fbx/FbxAnimationConverter.h"
#include "DirectX12/DrawObject/Fbx/FbxAnimationData.h"
#include "DirectX12/DrawObject/Fbx/FbxAnimationPlayer.h"
#include "DirectX12/DrawObject/Fbx/FbxStructure.h"
#include "DirectX12/DrawObject/Image/Image3DController.h"
#include "DirectX12/DrawObject/Image/ImageController.h"
#include "DirectX12/DrawObject/Image/ImageControllerStructure.h"
#include "DirectX12/DrawObject/Image/ImageObject.h"
#include "DirectX12/DrawObject/Image/Loader/ImageLoader.h"
#include "DirectX12/DrawObject/PMD/PMDController.h"
#include "DirectX12/DrawObject/PMD/PMDDataStructure.h"
#include "DirectX12/DrawObject/PMD/PMDLoader.h"
#include "DirectX12/DrawObject/PMD/PMDModel.h"
#include "DirectX12/DrawObject/PMD/PMXLoader.h"
#include "DirectX12/DrawObject/PMD/PMXModel.h"
#include "DirectX12/DrawObject/Primitive2D/Primitive2DManager.h"
#include "DirectX12/DrawObject/Primitive3D/PrimitiveController.h"
#include "DirectX12/DrawObject/Primitive3D/PrimitiveCreator.h"
#include "DirectX12/DrawObject/Primitive3D/PrimitiveObject.h"

#include "DirectX12/Input/DxInput.h"

#include "DirectX12/Light/LightObject.h"
#include "DirectX12/Light/DirectionalLight.h"

#include "DirectX12/Master/Dx12Ctrl.h"
#include "DirectX12/Master/NeedCall.h"

/**
*	Pipelinestate�t�H���_�C���N���[�h
*/
#include "DirectX12/PipelineState/DefaultPipelineState.h"
#include "DirectX12/PipelineState/NotCullPipelineState.h"
#include "DirectX12/PipelineState/PipelineStateObject.h"
#include "DirectX12/PipelineState/TransparentAfterPipelineState.h"
#include "DirectX12/PipelineState/WireFramePipelineState.h"

/**
*	RenderringPass�t�H���_�C���N���[�h
*/
#include "DirectX12/RenderingPass/Base/RenderingPassObject.h"
#include "DirectX12/RenderingPass/Manager/RenderingPassManager.h"

/**
*	Rootsignature�t�H���_�C���N���[�h
*/
#include "DirectX12/Rootsignature/PrimitiveNormalMapRootSignature.h"
#include "DirectX12/Rootsignature/PrimitiveRootSignature.h"
#include "DirectX12/Rootsignature/RootSignatureObject.h"
#include "DirectX12/Rootsignature/FbxNonShadeRootSignature.h"
#include "DirectX12/Rootsignature/Image3DRootSignature.h"

#include "DirectX12/Shader/HlslInclude.h"
#include "DirectX12/Shader/ShaderCompiler.h"
#include "DirectX12/Shader/ShaderDatasStructure.h"

#include "DirectX12/SwapChain/SwapChainObject.h"

#include "DirectX12/TextDevice/Dx12TextDevice.h"

#include "DirectX12/Texture/TextureLoader.h"
#include "DirectX12/Texture/TextureObject.h"

#include "DirectX12/Util/CharToWChar.h"
#include "DirectX12/Util/Dx12Getter.h"
#include "DirectX12/Util/File.h"
#include "DirectX12/Util/Rect.h"
#include "DirectX12/Util/Util.h"
#include "DirectX12/Util/XMFloatOperators.h"

/**
*	ViewDesc�t�H���_�C���N���[�h
*/
#include "DirectX12/ViewDesc/Dx12BufferViewDesc.h"
#include "DirectX12/ViewDesc/Dx12ConstantBufferViewDesc.h"
#include "DirectX12/ViewDesc/Dx12DepthStencilViewDesc.h"
#include "DirectX12/ViewDesc/Dx12RenderTargetViewDesc.h"
#include "DirectX12/ViewDesc/Dx12ShaderResourceViewDesc.h"
#include "DirectX12/ViewDesc/Dx12UnorderedAccessViewDesc.h"