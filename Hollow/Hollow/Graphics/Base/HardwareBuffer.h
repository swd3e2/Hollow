#pragma once

namespace Hollow {
	struct VERTEX_BUFFER_DESC
	{
		void* data;
		size_t stride;
		size_t size;
	};

	struct INDEX_BUFFER_DESC
	{
		void* data;
		size_t stride;
		size_t size;
	};

	class HardwareBuffer
	{
	protected:
		size_t stride;
		size_t size;
	public:
		HardwareBuffer(size_t size, size_t stride ) : size(size), stride(stride) {}
		virtual ~HardwareBuffer() {}

		inline size_t getSize() const { return size; }
		inline size_t getStride() const { return stride; }
	};

	class IndexBuffer
	{
	public:
		HardwareBuffer* mHardwareBuffer;
	public:
		IndexBuffer* create(const INDEX_BUFFER_DESC& desc);
	};

	class VertexBuffer
	{
	public:
		HardwareBuffer* mHardwareBuffer;
	public:
		VertexBuffer* create(const VERTEX_BUFFER_DESC& desc);
	};
}