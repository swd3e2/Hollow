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
		UINT32 stride;
		UINT32 size;

		VERTEX_BUFFER_DESC() = default;
		VERTEX_BUFFER_DESC(void* data, size_t size, UINT32 stride) :
			data(data), size(size), stride(stride)
		{}
	};

	struct INDEX_BUFFER_DESC
	{
		void* data;
		INDEX_FORMAT format;
		UINT32 size;

		INDEX_BUFFER_DESC() = default;
		INDEX_BUFFER_DESC(void* data, size_t size, INDEX_FORMAT format) :
			data(data), size(size), format(format)
		{}
	};

	class HardwareBuffer
	{
	protected:
		UINT32 stride;
		UINT32 size;
	public:
		HardwareBuffer(UINT32 size, UINT32 stride ) : size(size), stride(stride) {}
		virtual ~HardwareBuffer() {}

		inline UINT32 getSize() const { return size; }
		inline UINT32 getStride() const { return stride; }
		inline UINT32* getStridePtr() { return &stride; }
	};

	class IndexBuffer
	{
	public:
		HardwareBuffer* mHardwareBuffer;
	public:
		static s_ptr<IndexBuffer> create(const INDEX_BUFFER_DESC& desc);
		virtual ~IndexBuffer() { delete mHardwareBuffer; }
	};

	class VertexBuffer
	{
	public:
		HardwareBuffer* mHardwareBuffer;
	public:
		static s_ptr<VertexBuffer> create(const VERTEX_BUFFER_DESC& desc);
		virtual ~VertexBuffer() { delete mHardwareBuffer; }
	};
}