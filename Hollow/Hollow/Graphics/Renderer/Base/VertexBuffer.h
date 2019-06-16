#pragma once

#ifndef HW_VERTEX_BUFFER_H
#define HW_VERTEX_BUFFER_H

namespace Hollow {
	class VertexBuffer
	{
	protected:
		unsigned int stride;
		unsigned int bufferSize;
	public:
		VertexBuffer(unsigned int stride, unsigned int numIndices) : stride(stride), bufferSize(numIndices) {}
		virtual ~VertexBuffer() {}
		unsigned int getSize() const { return bufferSize; }
		const unsigned int Stride() const { return stride; }
		const unsigned int* StridePtr() const { return &stride; }
	};
}

#endif