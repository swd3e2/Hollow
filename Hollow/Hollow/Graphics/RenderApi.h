#pragma once

#ifndef HW_RENDER_API_H
#define HW_RENDER_API_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "HardwareBuffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "GPUBuffer.h"
#include "RenderTarget.h"
#include "InputLayout.h"
#include "DepthStencil.h"
#include "SamplerState.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "ShaderPipeline.h"
#include "Window.h"

namespace Hollow {
	/**
	 * Base render API class
	 */
	class RenderApi : public CModule<RenderApi>
	{
	protected:
		PrimitiveTopology mTopology;
		s_ptr<InputLayout> mCurrentLayout;
	public:
		/**
		 * Binds index buffer
		 * @param[in]	indexBuffer			Index buffer object
		 */
		virtual void setIndexBuffer(const s_ptr<IndexBuffer>& indexBuffer) = 0;

		/**
		 * Binds vertex buffer
		 * @param[in]	vertexBuffer		Vertex buffer object
		 */
		virtual void setVertexBuffer(const s_ptr<VertexBuffer>& vertexBuffer) = 0;
		
		/**
		 * Binds texture to texture unit
		 * @param[in]	textureUnit			Texture unit
		 * @param[in]	texture				Texture object
		 */
		virtual void setTexture(UINT textureUnit, const s_ptr<Texture>& texture) = 0;
		
		/**
		 * Unbinds texture
		 * @param[in]	textureUnit			Texture unit
		 */
		virtual void unsetTexture(UINT textureUnit) = 0;

		/**
		 * Binds specified color buffer from render target
		 * @param[in]	textureUnit			Texture unit
		 * @param[in]	renderTarget		Render target object
		 * @param[in]	targetNum			Number of render target color buffer
		 */
		virtual void setTextureColorBuffer(UINT, const s_ptr<RenderTarget>& renderTarget, UINT targetNum) = 0;

		/**
		 * Binds depth buffer from depth buffer
		 * @param[in]	textureUnit			Texture unit
		 * @param[in]	renderTarget		Render target object
		 */
		virtual void setTextureDepthBuffer(UINT, const s_ptr<RenderTarget>& renderTarget) = 0;

		/**
		 * Binds GPU buffer (DirectX: constant buffer, OpenGL: uniform buffer object)
		 * @param[in]	gpuBuffer		
		 */
		virtual void setGpuBuffer(const s_ptr<GPUBuffer>& gpuBuffer) = 0;

		/**
		 *
		 */
		virtual void draw(UINT count) = 0;

		/**
		 * Draw indexed
		 * @param[in]	count				Index count
		 */
		virtual void drawIndexed(UINT count) = 0;

		/**
		 * Draw instanced
		 */
		virtual void drawInstanced(UINT count, UINT instanceCount) = 0;

		/**
		 * Draw indexed instanced
		 */
		virtual void drawIndexedInstanced(UINT count, UINT instanceCount) = 0;

		/**
		 * Sets viewport
		 * @param[in]	x0					Left starting position
		 * @param[in]	y0					Top starting position
		 * @param[in]	x					Right ending position
		 * @param[in]	y					Bottom ending position
		 */
		virtual void setViewport(int x0, int y0, int x, int y) = 0;

		/**
		 * Binds render target
		 * @param[in]	renderTarget		Render target object
		 */
		virtual void setRenderTarget(const s_ptr<RenderTarget>& renderTarget) = 0;

		/**
		 * Clears render target
		 * @param[in]	renderTarget		Render target that needs to be cleared
		 * @param[in]	color				Color that render target will be filled with, float[4]
		 */
		virtual void clearRenderTarget(const s_ptr<RenderTarget>& renderTarget, const float* color) = 0;

		/**
		 * Presents a rendered image
		 */
		virtual void present() = 0;

		/**
		 * Binds input layout
		 */
		virtual void setInputLayout(const s_ptr<InputLayout>& layout) = 0;

		/**
		 * 
		 */
		virtual void setShaderPipeline(const s_ptr<ShaderPipeline>& shaderPipeline) = 0;

		/**
		 * Binds sampler to texture unit. OpenGL only.
		 * @param[in]	textureUnit			OpenGL texture unit that sampler will binded to
		 * @param[in]	sampler				Sampler object
		 */
		virtual void setTextureSampler(const int textureUnit, const s_ptr<SamplerState>& sampler) = 0;

		/**
		 * Sets sampler to sampler unit. DirectX only.
		 * @param[in]	samplerUnit			DirectX sampler unit
		 * @param[in]	sampler				Sampler object
		 */
		virtual void setSampler(const int samplerUnit, const s_ptr<SamplerState>& sampler) = 0;

		/**
		 * Sets depth stencil
		 * @param[in]	depthStencil		Depth stencil state
		 */
		virtual void setDepthStencilState(const s_ptr<DepthStencil>& depthStencil) = 0;

		/**
		 * Sets rasterizer state
		 * @param[in]	rasterizerState		Rasterizer state state
		 */
		virtual void setRasterizerState(const s_ptr<RasterizerState>& rasterizerState) = 0;

		/**
		 * Sets blend state
		 * @param[in]	blendState			Blend state
		 */
		virtual void setBlendState(const s_ptr<BlendState>& blendState) = 0;

		//s_ptr<Window> createWindow();

		virtual void setPrimitiveTopology(const PrimitiveTopology topology) = 0;

		virtual RendererType getRendererType() = 0;
	};
}

#endif