#pragma once

#include "Hollow/Platform.h"
#include "CommonTypes.h"

namespace Hollow {
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
		void* data = nullptr;
		IndexFormat format;
		UINT32 size;

		INDEX_BUFFER_DESC() = default;
		INDEX_BUFFER_DESC(void* data, size_t size, IndexFormat format) :
			data(data), size(size), format(format)
		{}
	};

	class HardwareBuffer
	{
	protected:
		UINT32 stride = 0;
		UINT32 size = 0;
		bool isDynamic = false;
	public:
		HardwareBuffer(UINT32 size, UINT32 stride ) : size(size), stride(stride) {}
		virtual ~HardwareBuffer() {}

		virtual void update(void* data, const int size) = 0;
		inline UINT32 getSize() const { return size; }
		inline UINT32 getStride() const { return stride; }
		inline UINT32* getStridePtr() { return &stride; }
	};

	class IndexBuffer
	{
	public:
		s_ptr<HardwareBuffer> mHardwareBuffer;
	public:
		void update(void* data, const int size)
		{
			mHardwareBuffer->update(data, size);
		}
		static s_ptr<IndexBuffer> create(const INDEX_BUFFER_DESC& desc);
	};

	class VertexBuffer
	{
	public:
		s_ptr<HardwareBuffer> mHardwareBuffer;
	public:
		void update(void* data, const int size)
		{
			mHardwareBuffer->update(data, size);
		}
		static s_ptr<VertexBuffer> create(const VERTEX_BUFFER_DESC& desc);
	};
}