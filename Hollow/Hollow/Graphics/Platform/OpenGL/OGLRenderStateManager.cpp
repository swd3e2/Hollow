#include "OGLRenderStateManager.h"

namespace Hollow {
	s_ptr<SamplerState> OGLRenderStateManager::createSamplerState(const SAMPLER_STATE_DESC& desc)
	{
		OGLSamplerState* sampler = new OGLSamplerState();

		glGenSamplers(1, &sampler->samplerObjectId);

		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_WRAP_S, OGLHelper::getAddressingMode(desc.addressingMode));
		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_WRAP_T, OGLHelper::getAddressingMode(desc.addressingMode));
		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_WRAP_R, OGLHelper::getAddressingMode(desc.addressingMode));

		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_MAG_FILTER, OGLHelper::getFilteringMode(desc.magFilterMode));
		glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_MIN_FILTER, OGLHelper::getMinMipFilteringMode(desc.minFilterModel, desc.mipFilterMode));

		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_MIN_LOD, desc.minLOD);
		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_MAX_LOD, desc.maxLOD);
		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_LOD_BIAS, desc.mipLODBias);
		glSamplerParameterf(sampler->samplerObjectId, GL_TEXTURE_MAX_ANISOTROPY, desc.maxAnisotropy);
		
		if (desc.comparisonFunction != ComparisonFunction::CMP_NEVER)
		{
			glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_COMPARE_FUNC, OGLHelper::getComparisonFunction(desc.comparisonFunction));
		}
		else
		{
			glSamplerParameteri(sampler->samplerObjectId, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}

		return s_ptr<SamplerState>(sampler);
	}

	s_ptr<RasterizerState> OGLRenderStateManager::createRasterizerState(const RASTERIZER_STATE_DESC& desc)
	{
		RasterizerState* rasterizer = new OGLRasterizerState(desc);
		return s_ptr<RasterizerState>(rasterizer);
	}

	s_ptr<BlendState> OGLRenderStateManager::createBlendState(const BLEND_STATE_DESC& desc)
	{
		BlendState* blendState = new OGLBlendState(desc);
		return s_ptr<BlendState>(blendState);
	}

	s_ptr<DepthStencil> OGLRenderStateManager::createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc)
	{
		DepthStencil* depthStencil = new OGLDepthStencilState(desc);
		return s_ptr<DepthStencil>(depthStencil);
	}
}

