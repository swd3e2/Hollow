#pragma once

#include <utility>
#include <vector>
#include <string>
#include "Hollow/Platform.h"

namespace Hollow {
	enum INPUT_DATA_TYPE
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	struct INPUT_LAYOUT_ELEMENT
	{
		INPUT_DATA_TYPE type;
		std::string name;
		size_t offset;

		INPUT_LAYOUT_ELEMENT(INPUT_DATA_TYPE type, const char* name) :
			type(type), name(name)
		{}

		const size_t getSize() const
		{
			switch (type)
			{
				case Hollow::Float:		return 4;
				case Hollow::Float2:	return 4 * 2;
				case Hollow::Float3:	return 4 * 3;
				case Hollow::Float4:	return 4 * 4;
				case Hollow::Int:		return 4;
				case Hollow::Int2:		return 4 * 2;
				case Hollow::Int3:		return 4 * 3;
				case Hollow::Int4:		return 4 * 4;
				case Hollow::Bool:		return 4;
			}
			return 0;
		}

		const size_t getNumberElements() const
		{
			switch (type)
			{
				case Hollow::Float:		return 1;
				case Hollow::Float2:	return 2;
				case Hollow::Float3:	return 3;
				case Hollow::Float4:	return 4;
				case Hollow::Int:		return 1;
				case Hollow::Int2:		return 2;
				case Hollow::Int3:		return 3;
				case Hollow::Int4:		return 4;
				case Hollow::Bool:		return 1;
			}
			return 0;
		}
	};
	
	class INPUT_LAYOUT_DESC
	{
	public:
		std::vector<INPUT_LAYOUT_ELEMENT> layout;
		size_t stride = 0;
	public:
		INPUT_LAYOUT_DESC(std::initializer_list<INPUT_LAYOUT_ELEMENT> elements) :
			layout(elements)
		{ 
			for (auto& it : layout) {
				it.offset = stride;
				stride += it.getSize();
			}
		}

		size_t getStride() const { return stride; }
	};


	class InputLayout
	{
	public:
		std::vector<INPUT_LAYOUT_ELEMENT> layout;
	private:
		size_t stride;
	public:
		static s_ptr<InputLayout> create(const INPUT_LAYOUT_DESC& desc);
		size_t getStride() const { return stride; }
	};
}