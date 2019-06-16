#pragma once

#ifndef HW_BUFFER_H
#define HW_BUFFER_H

namespace Hollow {
	class Buffer
	{
	public:
		virtual size_t BufferSize() const = 0;
	};
}

#endif