#pragma once

#include <utility>
#include <vector>

namespace Hollow {
	enum InputDataType
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

	struct InputLayoutElement
	{
		InputDataType type;
		size_t offset;

		InputLayoutElement(InputDataType type) :
			type(type)
		{}

		size_t getSize() 
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

		size_t getNumberElements()
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

	class InputLayout
	{
	public:
		std::vector<InputLayoutElement> layout;
		size_t stride = 0;
	public:
		InputLayout(std::initializer_list<InputLayoutElement> elements) :
			layout(elements)
		{ 
			for (auto& it : layout) {
				stride += it.getSize();
			}
		}
	};
}