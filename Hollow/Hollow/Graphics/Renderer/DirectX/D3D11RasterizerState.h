#pragma once

#ifndef HW_D3D11_RASTERIZER_STATE_H
#define HW_D3D11_RASTERIZER_STATE_H

#include "D3D11Prerequisites.h"

class D3D11RasterizerState
{
private:
	ID3D11RasterizerState* m_RasterizerState;
public:
	D3D11RasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);
	ID3D11RasterizerState* GetRasterizerState();
};

#endif