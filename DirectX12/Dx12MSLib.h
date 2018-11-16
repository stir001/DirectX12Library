#pragma once

/**
*	Animationフォルダインクルード
*/
#include "Animation/Animation.h"
#include "Animation/AnimationPlayer.h"
#include "Animation/AnimationPlayerManager.h"
#include "Animation/VMD/VMDLoader.h"
#include "Animation/VMD/VMDMotion.h"
#include "Animation/VMD/VMDPlayer.h"

/**
*	Bufferフォルダインクルード
*/
#include "Buffer/Dx12BufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Buffer/AdditionalConstatntBuffer.h"
#include "Buffer/DepthBufferObject.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/RendertargetObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "Buffer/VertexBufferObject.h"

#include "Camera/Dx12Camera.h"
#include "Camera/CameraHolder.h"

#include "CommandList/Dx12CommandList.h"

#include "DescriptorHeap/Dx12DescriptorHeapObject.h"

/**
*	DrawObjectフォルダインクルード
*/
#include "DrawObject/DrawController3D.h"
#include "DrawObject/DrawObjectController.h"
#include "DrawObject/DrawObjectLoader.h"
#include "DrawObject/Model.h"
#include "DrawObject/Transform3DCalculator.h"
#include "DrawObject/TransformMatrixCalculator.h"
#include "DrawObject/Fbx/FbxLoader.h"
#include "DrawObject/Fbx/FbxModel.h"
#include "DrawObject/Fbx/FbxModelController.h"
#include "DrawObject/Fbx/FbxModelDataCoverter.h"
#include "DrawObject/Fbx/FbxMotionConverter.h"
#include "DrawObject/Fbx/FbxMotionData.h"
#include "DrawObject/Fbx/FbxMotionPlayer.h"
#include "DrawObject/Fbx/FbxStructure.h"
#include "DrawObject/Image/Image3DController.h"
#include "DrawObject/Image/ImageController.h"
#include "DrawObject/Image/ImageControllerStructure.h"
#include "DrawObject/Image/ImageObject.h"
#include "DrawObject/Image/Loader/ImageLoader.h"
#include "DrawObject/PMD/PMDController.h"
#include "DrawObject/PMD/PMDDataStructure.h"
#include "DrawObject/PMD/PMDLoader.h"
#include "DrawObject/PMD/PMDModel.h"
#include "DrawObject/PMD/PMXLoader.h"
#include "DrawObject/PMD/PMXModel.h"
#include "DrawObject/Primitive2D/IPrimitive2D.h"
#include "DrawObject/Primitive2D/Primitive2DLine.h"
#include "DrawObject/Primitive2D/Primitive2DManager.h"
#include "DrawObject/Primitive3D/PrimitiveController.h"
#include "DrawObject/Primitive3D/PrimitiveCreator.h"
#include "DrawObject/Primitive3D/PrimitiveCube.h"
#include "DrawObject/Primitive3D/PrimitiveObject.h"
#include "DrawObject/Primitive3D/PrimitivePlane.h"

#include "Input/DxInput.h"

#include "Light/LightObject.h"
#include "Light/DirectionalLight.h"

#include "Master/Dx12Ctrl.h"
#include "Master/NeedCall.h"

/**
*	Pipelinestateフォルダインクルード
*/
#include "PipelineState/PipelineStateObject.h"
#include "PipelineState/Image2DPipelineState.h"
#include "PipelineState/PrimitivePipelineState.h"

/**
*	RenderringPathフォルダインクルード
*/
#include "RenderingPath/FirstPathObject.h"
#include "RenderingPath/Base/RenderingPathObject.h"
#include "RenderingPath/Manager/RenderingPathManager.h"

/**
*	Rootsignatureフォルダインクルード
*/
#include "Rootsignature/PrimitiveNormalMapRootSignature.h"
#include "Rootsignature/PrimitiveRootSignature.h"
#include "Rootsignature/RootSignatureObject.h"

#include "Shader/HlslInclude.h"
#include "Shader/ShaderCompiler.h"
#include "Shader/ShaderDatasStructure.h"

#include "SwapChain/SwapChainObject.h"

#include "TextDevice/Dx12TextDevice.h"

#include "Texture/TextureLoader.h"
#include "Texture/TextureObject.h"

#include "Util/CharToWChar.h"
#include "Util/Dx12Getter.h"
#include "Util/File.h"
#include "Util/Rect.h"
#include "Util/Util.h"
#include "Util/XMFloatOperators.h"

/**
*	ViewDescフォルダインクルード
*/
#include "ViewDesc/Dx12BufferViewDesc.h"
#include "ViewDesc/Dx12ConstantBufferViewDesc.h"
#include "ViewDesc/Dx12DepthStencilViewDesc.h"
#include "ViewDesc/Dx12RenderTargetViewDesc.h"
#include "ViewDesc/Dx12ShaderResourceViewDesc.h"
#include "ViewDesc/Dx12UnorderedAccessViewDesc.h"
