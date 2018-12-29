#include "RenderSystem.h"

RenderSystem::RenderSystem(HWND * hwnd, int width, int height)
{
	Hollow::Log::GetCoreLogger()->debug("RenderSystem: Initialized.");
	m_Renderer = new Hollow::Core::Graphics::HollowDirectXRenderer(hwnd, width, height);

	this->camera = new Camera();
	this->camera->SetProjectionValues(85.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	this->camera->SetPosition(3.0f, 2.0f, -5.0f);

	transformConstantBuffer.Init(this->m_Renderer->GetDevice(), this->m_Renderer->GetDeviceContext());
	WVPConstantBuffer.Init(this->m_Renderer->GetDevice(), this->m_Renderer->GetDeviceContext());

	D3D11_INPUT_ELEMENT_DESC bxlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(bxlayout);

	vertexShader = new VertexShader(this->m_Renderer->GetDevice(), L"Sandbox/Resources/Shaders/vs.hlsl", bxlayout, numElements);
	pixelShader = new PixelShader(this->m_Renderer->GetDevice(), L"Sandbox/Resources/Shaders/ps.hlsl");

	std::vector<SimpleVertex> vertices;
	vertices.push_back({ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back({ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back({ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back({ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });

	vertices.push_back({ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) });
	vertices.push_back({ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back({ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
	vertices.push_back({ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) });

	std::vector<unsigned int> vindices;
	unsigned int * indices = new unsigned int[36]{
			2, 1, 0,
			2, 0, 3,
			2, 5, 1,
			5, 7, 1,
			4, 2, 3,
			4, 5, 2,
			4, 3, 0,
			4, 0, 6,
			1, 6, 0,
			1, 7, 6,
			5, 6, 7,
			5, 4, 6
	};

	for (int i = 0; i < 36; i++)
		vindices.push_back(indices[i]);

	Hollow::VertexBuffer<SimpleVertex> vBuffer(m_Renderer->GetDevice(), vertices.data(), vertices.size());
	Hollow::IndexBuffer<unsigned int> iBuffer(m_Renderer->GetDevice(), vindices.data(), vindices.size());

	m_Renderer->SetVertexBuffer<SimpleVertex>(&vBuffer);
	m_Renderer->SetIndexBuffer<unsigned int>(&iBuffer);
}

void RenderSystem::PreUpdate(float_t dt) 
{
	this->camera->Update();
	this->UpdateWVP();

	this->m_Renderer->Clear();
	this->m_Renderer->PreUpdateFrame();

	this->m_Renderer->SetContantBuffer<WVP>(HOLLOW_CONST_BUFFER_WVP_SLOT, &WVPConstantBuffer);
	this->m_Renderer->SetVertexShader(vertexShader);
	this->m_Renderer->SetPixelShader(pixelShader);
}

void RenderSystem::Update(float_t dt, std::vector<GameObject*>& gameObjects)
{
	for (auto object : gameObjects) {
		//MeshComponent * meshComponent = object->GetComponent<MeshComponent>();
		PositionComponent * posComponent = object->GetComponent<PositionComponent>();

		//if (meshComponent == nullptr || posComponent == nullptr) continue;

		this->UpdateTransform(posComponent);
		this->m_Renderer->SetContantBuffer<Transform>(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, &transformConstantBuffer);

		m_Renderer->Draw(36);
	}
}

void RenderSystem::PostUpdate(float_t dt) 
{
	m_Renderer->Present();
}

void RenderSystem::UpdateTransform(PositionComponent * comp)
{
	transformConstantBuffer.data.transform = XMMatrixTranspose(XMMatrixTranslation(comp->position.x, comp->position.y, comp->position.z));;
	transformConstantBuffer.Update();
}

void RenderSystem::UpdateWVP()
{
	WVPConstantBuffer.data.WVP = XMMatrixTranspose(XMMatrixIdentity()
		* camera->GetViewMatrix() 
		* camera->GetProjectionMatrix());
	WVPConstantBuffer.Update();
}
