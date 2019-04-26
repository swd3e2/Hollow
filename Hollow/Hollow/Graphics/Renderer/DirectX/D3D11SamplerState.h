#pragma once
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "D3D11RenderApi.h"
#include "Hollow/Graphics/Renderer/Base/SamplerState.h"

class HOLLOW_API D3D11SamplerState : public SamplerState
{
private:
	ID3D11SamplerState* m_SamplerState;
public:
	D3D11SamplerState(D3D11_TEXTURE_ADDRESS_MODE mode);

	~D3D11SamplerState();

	inline ID3D11SamplerState* const* GetSamplerState() { return &m_SamplerState; }
};