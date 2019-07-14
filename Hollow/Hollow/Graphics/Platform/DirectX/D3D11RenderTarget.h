#pragma once

#ifndef HW_D3D11_RENDER_TARGET_H
#define HW_D3D11_RENDER_TARGET_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/RenderTarget.h"

namespace Hollow {
	class D3D11RenderTarget : public RenderTarget
	{
	public:
		friend class D3D11RenderTargetManager;
	public:
		D3D11RenderTarget(int width, int height, int count) :
			RenderTarget(width, height, count),
			renderTarget(nullptr), m_BackBuffer(nullptr), m_ShaderResourceView(nullptr)
		{}
		~D3D11RenderTarget() {
			for (int i = 0; i < count; i++) {
				SAFE_RELEASE(renderTarget[i]);
				SAFE_RELEASE(m_BackBuffer[i]);
				SAFE_RELEASE(m_ShaderResourceView[i]);
			}
			
			SAFE_RELEASE(m_DepthStencilView);
			SAFE_RELEASE(m_DepthStencilBuffer);
			SAFE_RELEASE(m_DepthResourceView);
		}

		inline ID3D11RenderTargetView** GetRenderTaget() const { return renderTarget; }
		inline ID3D11ShaderResourceView** GetShaderResourceView() const { return m_ShaderResourceView; }

		inline ID3D11DepthStencilView* GetDepthStencilView() const { return m_DepthStencilView; }
		inline ID3D11ShaderResourceView* GetDepthStencilResource() const { return m_DepthResourceView; }
		virtual Vector4 readPixel(int x, int y) override;
	private:
		ID3D11RenderTargetView** renderTarget;
		ID3D11Texture2D** m_BackBuffer;
		ID3D11ShaderResourceView** m_ShaderResourceView;

		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11ShaderResourceView* m_DepthResourceView;
	};
}

#endif