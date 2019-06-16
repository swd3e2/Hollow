#pragma once

#ifndef HW_INDEX_BUFFER_H
#define HW_INDEX_BUFFER_H

namespace Hollow {
	class IndexBuffer
	{
	protected:
		unsigned int stride;
		unsigned int bufferSize;
	public:
		IndexBuffer(unsigned int stride, unsigned int numIndices) : stride(stride), bufferSize(numIndices) {}
		virtual ~IndexBuffer() {}
		unsigned int getSize() const { return bufferSize; }
		const unsigned int Stride() const { return stride; }
		const unsigned int* StridePtr() const { return &stride; }
	};
}

#endif