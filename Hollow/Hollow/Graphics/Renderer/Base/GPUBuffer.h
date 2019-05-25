#pragma once

class GPUBuffer
{
protected:
	unsigned int location;
	unsigned int size;
public:
	GPUBuffer(unsigned int location, unsigned int size) :
		location(location), size(size)
	{}

	unsigned int getSize() const { return size; }
	unsigned int getLocation() const { return location; }

	virtual void update(void* data) = 0;
};