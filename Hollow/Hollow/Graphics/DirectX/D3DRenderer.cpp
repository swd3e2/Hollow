#include "D3DRenderer.h"

D3DRenderer::D3DRenderer() :
	window(GetModuleHandle(NULL), SCREEN_WIDTH, SCREEN_HEIGHT)
{
	HRESULT hr = S_OK;
	RECT rc;

	GetClientRect(*window.getHWND(), &rc);
	this->width = rc.right - rc.left;
	this->height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.OutputWindow = *window.getHWND();
		
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_1;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, m_SwapChain.GetAddressOf(),
		&m_Device, &m_featureLevel, m_DeviceContext.GetAddressOf());

	if (hr != S_OK) {
		Hollow::Log::GetCoreLogger()->error("RenderSystem: Can't create DeviceAndSwapChain!");
	}

	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_DeviceContext->RSSetViewports(1, &vp);
	m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_RenderTarget = new D3DRenderTarget(m_Device.Get(), m_DeviceContext.Get(), m_SwapChain.Get(), width, height);
	m_DepthStencil = new D3DDepthStencil(m_Device.Get(), width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

	m_BlendStateTransparancy = new D3DBlendState(m_Device.Get());

	m_SamplerStateWrap = new D3DSamplerState(m_Device.Get(), D3D11_TEXTURE_ADDRESS_WRAP);
	m_SamplerStateClamp = new D3DSamplerState(m_Device.Get(), D3D11_TEXTURE_ADDRESS_CLAMP);

	camera = new Camera();
	camera->SetProjectionValues(85.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	m_WVPConstantBuffer = new D3DConstantBuffer(m_Device.Get(), m_DeviceContext.Get(), sizeof(WVP));

	std::vector<std::string>* shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/");
	for (auto& it : *shaders)
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
			vShaders.push_back(new D3DVertexShader(m_Device.Get(), "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/" + it));

	shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/");
	for (auto& it : *shaders)
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
			pShaders.push_back(new D3DPixelShader(m_Device.Get(), "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/" + it));
}

D3DRenderer::~D3DRenderer()
{
	m_Device->Release();
	m_DeviceContext->Release();
	m_SwapChain->Release();
	delete	m_WVPConstantBuffer;
	delete	m_TransformConstantBuffer;
	delete	m_BlendStateTransparancy;
	delete	m_SamplerStateWrap;
	delete	m_SamplerStateClamp;
	delete	m_RenderTarget;
	delete	m_DepthStencil;
}

void D3DRenderer::PreUpdateFrame()
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTarget->GetMainRenderTaget(), ClearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_DeviceContext->PSSetSamplers(0, 1, m_SamplerStateWrap->GetSamplerState());
	m_DeviceContext->PSSetSamplers(1, 1, m_SamplerStateClamp->GetSamplerState());
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTarget->GetAddressOfMainRenderTaget(), m_DepthStencil->GetDepthStencilView());
	camera->Update();
	UpdateWVP();
	SetVertexShader(vShaders[0]);
	SetPixelShader(pShaders[0]);
	SetContantBuffer(0, m_WVPConstantBuffer);
}

size_t D3DRenderer::createRenderable(Mesh * mesh)
{
	D3DRenderable* renderable = new D3DRenderable();

	for (auto& it : mesh->objects) {
		RenderableObject* object = new RenderableObject();
		object->buffer = new D3DBuffer(m_Device.Get(), it->data, sizeof(SimpleVertex), it->numVertices, D3D11_BIND_VERTEX_BUFFER);
		D3DMaterial* mat = new D3DMaterial();
		D3DTexture* tex = new D3DTexture();
		tex->CreateTexture(m_Device.Get(), m_DeviceContext.Get(), it->material.diffuse_texture.c_str());
		mat->SetDiffuseTexture(tex);
		object->material = mat;
		renderable->renderableObjects.push_back(object);
	}

	renderables.push_back(renderable);
	return 1;
}

void D3DRenderer::Draw(RenderableObject * object)
{
	if (object->material->diffuseTexture->active) {
		this->m_DeviceContext->PSSetShaderResources(0, 1, &object->material->diffuseTexture->m_TextureShaderResource);
	}
	this->m_DeviceContext->IASetVertexBuffers(0, 1, object->buffer->GetAddressOf(), object->buffer->StridePtr(), &this->offset);
	m_DeviceContext->Draw(object->buffer->BufferSize(), 0);
}

void D3DRenderer::PostUpdateFrame()
{
	m_SwapChain->Present(1, 0);
}

void D3DRenderer::UpdateWVP()
{
	m_wvp.WVP = XMMatrixTranspose(XMMatrixIdentity()
		* camera->GetViewMatrix()
		* camera->GetProjectionMatrix());

	m_WVPConstantBuffer->Update(&m_wvp, sizeof(m_wvp));
}

bool D3DRenderer::processMessage()
{
	return window.ProcessMessage();
}

void D3DRenderer::Update()
{
	PreUpdateFrame();

	for (auto& it : renderables)
		for (auto& buff : it->renderableObjects)
			Draw(buff);

	PostUpdateFrame();
}
