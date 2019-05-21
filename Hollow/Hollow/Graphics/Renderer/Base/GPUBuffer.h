#pragma once

#include <string>

struct GPUBufferStruct
{
	std::string name; // ogl

	unsigned int size;
	unsigned int count;
};

class GPUBuffer
{
	unsigned int location; // dx
};