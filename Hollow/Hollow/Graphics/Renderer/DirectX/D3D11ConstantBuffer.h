#pragma once

#ifndef HW_D3D11_CONSTANT_BUFFER_H
#define HW_D3D11_CONSTANT_BUFFER_H

#include "Hollow/Platform.h"
#include "D3D11Prerequisites.h"

class HOLLOW_API D3D11ConstantBuffer
{
private:
	D3D11ConstantBuffer(const D3D11ConstantBuffer &rhs);
private:
	ID3D11Buffer* m_Buffer;
	UINT size;
public:
	/*
	 * @param UINT size size of structure 
	 */
	D3D11ConstantBuffer(UINT size);
	~D3D11ConstantBuffer();
	
	ID3D11Buffer * const* GetAddressOf() const { return &m_Buffer; }
	bool Update(void* data);
};

#endif