#pragma once

namespace Hollow {
	enum HARDWARE_BUFFER_USAGE
	{
		INDEX,
		VERTEX
	};

	struct HARDWARE_BUFFER_DESC
	{
		void* data;
		size_t stride;
		size_t size;
		HARDWARE_BUFFER_USAGE usage;
	};

	class HardwareBuffer
	{
	protected:
		size_t stride;
		size_t size;
	public:
		HardwareBuffer(size_t size, size_t stride ) : size(size), stride(stride) {}
		virtual ~HardwareBuffer() {}

		HardwareBuffer* create(const HARDWARE_BUFFER_DESC& desc);

		inline size_t getSize() const { return size; }
		inline size_t getStride() const { return stride; }
	};
}