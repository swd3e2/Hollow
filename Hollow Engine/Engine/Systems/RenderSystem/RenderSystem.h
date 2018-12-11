#ifndef __RENDER_SYSTEM__
#define __RENDER_SYSTEM__
#pragma once
#include "../System.h"
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <directXMath.h>
#include <wrl/client.h>

#include "Engine/Common/Log.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/BufferTemplate/ConstantBuffer.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/BufferTemplate/IndexBuffer.h"
#include "Engine/Graphics/BufferTemplate/VertexBuffer.h"

namespace Hollow {

	struct ConstantBufferStruct
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};
	struct SimpleVertex
	{
		DirectX::XMFLOAT4 Pos;
		DirectX::XMFLOAT4 Color;
	};
	class RenderSystem : System<RenderSystem>
	{
	private:
		HWND * hWnd;
		int height;
		int width;
		bool fullscreen = false;

		D3D_DRIVER_TYPE				m_driverType = D3D_DRIVER_TYPE_HARDWARE;
		D3D_FEATURE_LEVEL			m_featureLevel = D3D_FEATURE_LEVEL_11_1;
		ID3D11ShaderResourceView *const pSRV[1] = { NULL };

		Microsoft::WRL::ComPtr<ID3D11Device>				m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Resource>				m_depthResource;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_pVertexLayout;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_RasterizerState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_SamplerStateWrap;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_SampleStateClamp;
		Microsoft::WRL::ComPtr<ID3D11BlendState>			Transparency;

		VertexBuffer<SimpleVertex>*							vertexBuffer;
		IndexBuffer<unsigned int>*							indexBuffer;

		VertexShader*										vertexShader;
		PixelShader*										pixelShader;
		RenderTarget*										renderTarget;
		DepthStencil*										depthStencil;
		Camera*												camera;
		ConstantBuffer<ConstantBufferStruct>				constantBuffer;
	private:
		void CreateSwapChain();
		void InitDevices();
		void UpdateWVP();
		void CreateRasterizerState();
		void CreateSamplerDesc();
		void DefineBlending();
	public:
		RenderSystem(HWND * hwnd, int height, int width);
		virtual void PreUpdate(float_t dt) override;

		virtual void Update(float_t dt) override;

		virtual void PostUpdate(float_t dt) override;
	};

}

#endif