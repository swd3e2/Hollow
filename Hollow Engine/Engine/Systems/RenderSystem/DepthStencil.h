#pragma once

#ifndef __DEPTH_STENCIL__
#define __DEPTH_STENCIL__

#include <d3d11.h>
#include <wrl/client.h>
#include "../../Common/Log.h"

namespace Hollow {
	
	class DepthStencil
	{
	public:
		DepthStencil(ID3D11Device * device, int width, int height, DXGI_FORMAT format, int samplecount)
		{
			HRESULT hr = S_OK;

			DXGI_FORMAT resformat = GetDepthResourceFormat(format);
			DXGI_FORMAT srvformat = GetDepthSRVFormat(format);

			D3D11_TEXTURE2D_DESC desc;
			desc.ArraySize = 1;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.Format = resformat;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.MiscFlags = 0;
			desc.SampleDesc.Count = samplecount;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.Width = width;

			hr = device->CreateTexture2D(&desc, NULL, m_DepthStencilBuffer.GetAddressOf());
			if (hr != S_OK) {
				Hollow::Log::GetCoreLogger()->error("DepthStencil: Cant create Texture2D!");
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC ddesc;
			ZeroMemory(&ddesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			ddesc.Format = format;
			ddesc.ViewDimension = samplecount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
			ddesc.Texture2D.MipSlice = 0;

			hr = device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &ddesc, m_DepthStencilView.GetAddressOf());
			if (hr != S_OK) {
				Hollow::Log::GetCoreLogger()->error("DepthStencil: Cant create DepthStencilView!");
			}

			D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			srvd.Format = srvformat;
			srvd.ViewDimension = samplecount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
			srvd.Texture2D.MipLevels = 1;
			srvd.Texture2D.MostDetailedMip = 0;

			hr = device->CreateShaderResourceView(m_DepthStencilBuffer.Get(), &srvd, &mDepthResourceView);
			if (hr != S_OK) {
				Hollow::Log::GetCoreLogger()->error("DepthStencil: Cant create ShaderResourceView!");
			}
		}

		inline ID3D11DepthStencilView * GetDepthStencilView() { return m_DepthStencilView.Get(); }
		inline ID3D11DepthStencilView ** GetAddressOfDepthStencilView() { return m_DepthStencilView.GetAddressOf(); }

		inline ID3D11DepthStencilState * GetDepthStencilState() { return m_DepthStencilState.Get(); }
		inline ID3D11ShaderResourceView * GetDepthStencilResource() { return mDepthResourceView; }
		inline ID3D11ShaderResourceView ** GetAddressOfDepthStencilResource() { return &mDepthResourceView; }
	private:
		DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT depthformat)
		{
			DXGI_FORMAT resformat;
			switch (depthformat)
			{
			case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
				resformat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
				break;
			case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
				resformat = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
				break;
			case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
				resformat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
				break;
			case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
				resformat = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
				break;
			}

			return resformat;
		}

		DXGI_FORMAT GetDepthSRVFormat(DXGI_FORMAT depthformat)
		{
			DXGI_FORMAT srvformat;
			switch (depthformat)
			{
			case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
				srvformat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
				break;
			case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
				srvformat = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				break;
			case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
				srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
				break;
			case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
				srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
				break;
			}
			return srvformat;
		}
	private:
		float m_zNear, m_zFar;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_DepthStencilState;
		ID3D11ShaderResourceView *	mDepthResourceView;
	};
}


#endif