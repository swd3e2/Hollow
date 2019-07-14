#pragma once

#include <utility>
#include <vector>
#include <string>

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

		std::vector<INPUT_LAYOUT_ELEMENT>::iterator begin() {
			return layout.begin();
		}

		std::vector<INPUT_LAYOUT_ELEMENT>::iterator end() {
			return layout.end();
		}
	};


	class InputLayout
	{
		
	};
}

/*
		glVertexAttribFormat(0, 4, GL_FLOAT, GL_FALSE, 0)
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 16)
		glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 24)

		glVertexAttribPointer(POSITION_LOCATION,	3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(TEX_COORD_LOCATION,	2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(NORMAL_LOCATION,		3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glVertexAttribPointer(TANGENT_LOCATION,		3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
		glVertexAttribPointer(BITANGENT_LOCATION,	3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);
		glVertexAttribIPointer(BONE_ID_LOCATION,	4, GL_INT,	sizeof(Vertex), (const GLvoid*)56);
		glVertexAttribPointer(WEIGHTS_LOCATION,		4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)72);

		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,	   0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,	   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEID",     0, DXGI_FORMAT_R32G32B32A32_SINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	*/