#pragma once

#ifndef HW_D3D11_RENDER_TARGET_H
#define HW_D3D11_RENDER_TARGET_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/RenderTarget.h"

namespace Hollow {
	class D3D11RenderTarget : public RenderTarget
	{
	public:
		friend class D3D11RenderTargetManager;
	public:
		D3D11RenderTarget(int width, int height, int count) :
			RenderTarget(width, height, count)
		{}

		virtual ~D3D11RenderTarget() {
			for (int i = 0; i < getCount(); i++) {
				SAFE_RELEASE(m_RenderTarget[i]);
				SAFE_RELEASE(m_BackBuffer[i]);
				SAFE_RELEASE(m_ShaderResourceView[i]);
			}
			
			SAFE_RELEASE(m_DepthStencilView);
			SAFE_RELEASE(m_DepthStencilBuffer);
			SAFE_RELEASE(m_DepthResourceView);
		}

		inline ID3D11RenderTargetView** getRenderTaget() const { return m_RenderTarget; }
		inline ID3D11ShaderResourceView** getShaderResourceView() const { return m_ShaderResourceView; }

		inline ID3D11DepthStencilView* getDepthStencilView() const { return m_DepthStencilView; }
		inline ID3D11ShaderResourceView* getDepthStencilResource() const { return m_DepthResourceView; }
		virtual Vector4 readPixel(int x, int y) override;
	private:
		ID3D11RenderTargetView** m_RenderTarget = nullptr;
		ID3D11Texture2D** m_BackBuffer = nullptr;
		ID3D11ShaderResourceView** m_ShaderResourceView = nullptr;

		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
		ID3D11ShaderResourceView* m_DepthResourceView = nullptr;
	};
}

#endif