#pragma once

#include "Hollow/Platform.h"

namespace Hollow {
	enum class INDEX_FORMAT
	{
		UINT,
		INT
	};

	struct VERTEX_BUFFER_DESC
	{
		void* data;
		UINT stride;
		UINT size;

		VERTEX_BUFFER_DESC() = default;
		VERTEX_BUFFER_DESC(void* data, size_t size, UINT stride) :
			data(data), size(size), stride(stride)
		{}
	};

	struct INDEX_BUFFER_DESC
	{
		void* data;
		INDEX_FORMAT format;
		UINT size;
		

		INDEX_BUFFER_DESC() = default;
		INDEX_BUFFER_DESC(void* data, size_t size, INDEX_FORMAT format) :
			data(data), size(size), format(format)
		{}
	};

	class HardwareBuffer
	{
	protected:
		UINT stride;
		UINT size;
	public:
		HardwareBuffer(UINT size, UINT stride ) : size(size), stride(stride) {}
		virtual ~HardwareBuffer() {}

		inline UINT getSize() const { return size; }
		inline UINT getStride() const { return stride; }
		inline UINT* getStridePtr() { return &stride; }
	};

	class IndexBuffer
	{
	public:
		HardwareBuffer* mHardwareBuffer;
	public:
		static IndexBuffer* create(const INDEX_BUFFER_DESC& desc);
		virtual ~IndexBuffer() { delete mHardwareBuffer; }
	};

	class VertexBuffer
	{
	public:
		HardwareBuffer* mHardwareBuffer;
	public:
		static VertexBuffer* create(const VERTEX_BUFFER_DESC& desc);
		virtual ~VertexBuffer() { delete mHardwareBuffer; }
	};
}