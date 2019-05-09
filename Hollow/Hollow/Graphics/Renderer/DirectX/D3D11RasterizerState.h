#pragma once

#ifndef HW_D3D11_RASTERIZER_STATE_H
#define HW_D3D11_RASTERIZER_STATE_H

#include "D3D11Prerequisites.h"

class D3D11RasterizerState
{
private:
	ID3D11RasterizerState* m_RasterizerState;
public:
	D3D11RasterizerState();
	ID3D11RasterizerState* GetRasterizerState();
};

#endif