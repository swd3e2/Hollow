#pragma once
#include "Hollow/ECS/Systems/System.h"
#include <windows.h>
#include <d3d11.h>
#include <directXMath.h>
#include <wrl/client.h>

#include "Hollow/Common/Log.h"
#include "Hollow/Graphics/RenderTarget.h"
#include "Hollow/Graphics/DepthStencil.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Graphics/BufferTemplate/ConstantBuffer.h"
#include "Hollow/Graphics/Shaders/VertexShader.h"
#include "Hollow/Graphics/Shaders/PixelShader.h"
#include "Hollow/Graphics/BufferTemplate/IndexBuffer.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Hollow/Graphics/SimpleVertex.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/MeshComponent.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Hollow/ECS/ComponentManager.h"

using namespace Hollow;

struct ConstantBufferStruct
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
	DirectX::XMMATRIX transform;
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
	void UpdateWVP(PositionComponent * comp);
	void CreateRasterizerState();
	void CreateSamplerDesc();
	void DefineBlending();
public:
	RenderSystem(HWND * hwnd, int height, int width);

	virtual void PreUpdate(float_t dt) override;

	virtual void Update(float_t dt, std::vector<GameObject*>& gameObjects);

	virtual void PostUpdate(float_t dt) override;

	inline ID3D11Device * GetDevice() { return this->m_Device.Get(); }
	inline ID3D11DeviceContext * GetDeviceContext() { return this->m_DeviceContext.Get(); }
};

