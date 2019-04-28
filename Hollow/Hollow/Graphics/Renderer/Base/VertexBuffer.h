#pragma once

#ifndef HW_I_VERTEX_BUFFER_H
#define HW_I_VERTEX_BUFFER_H

class VertexBuffer
{
protected:
	unsigned int stride;
	unsigned int bufferSize;
public:
	VertexBuffer(unsigned int stride, unsigned int numIndices) : stride(stride), bufferSize(numIndices) {}
	unsigned int getSize() const { return bufferSize; }
	const unsigned int Stride() const { return stride; }
	const unsigned int* StridePtr() const { return &stride; }
};

#endif