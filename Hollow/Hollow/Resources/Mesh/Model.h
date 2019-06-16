#pragma once

#ifndef HW_MODEL_H
#define HW_MODEL_H

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "Hollow/Resources/Material.h"

namespace Hollow {
	class Model
	{
	public:
		std::string name;
		VertexBuffer* vBuffer;
		IndexBuffer* iBuffer;
		Material* material;
	public:
		Model() {}
		~Model()
		{
			delete vBuffer;
			delete iBuffer;
			delete material;
		}
	};
}

#endif