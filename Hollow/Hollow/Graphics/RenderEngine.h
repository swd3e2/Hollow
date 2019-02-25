#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include <vector>
#include "IRenderable.h"
#include "DirectX/D3DRenderApi.h"
#include "IRenderApi.h"
#include "Camera.h"
#include "DirectX/D3DConstantBuffer.h"

namespace Hollow {

	struct WVP
	{
		DirectX::XMMATRIX WVP;
	};

	struct Transform
	{
		DirectX::XMMATRIX transform;
		float id;
		bool selected;
	};

	class RenderEngine
	{
	protected:
		std::vector<IRenderable>			m_RenderableList;
		IRenderApi*							renderApi;
		Camera*								camera;
		WVP									wvp;
		D3DConstantBuffer*					WVPConstantBuffer;
		D3DConstantBuffer*					transformConstantBuffer;
	public:
		RenderEngine(HWND* hwnd, int width, int height)
		{
			renderApi = new D3DRendererApi(hwnd, width, height);
			WVPConstantBuffer = new D3DConstantBuffer();
		}
		void Render(int renderableId)
		{
			UpdateWVP();
		}

		IRenderable* GetRenderable(int renderableId)
		{
			return &m_RenderableList[renderableId];
		}

		void UpdateWVP()
		{
			wvp.WVP = XMMatrixTranspose(XMMatrixIdentity()
				* camera->GetViewMatrix()
				* camera->GetProjectionMatrix());

			WVPConstantBuffer->Update(&wvp, sizeof(wvp));
		}
	};
}