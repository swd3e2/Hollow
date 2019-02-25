#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"

namespace Hollow {


	class RenderEngine
	{
	protected:
		Hollow::Containers::Vector<Renderable> m_RenderableList;
	public:
		virtual void Render(int renderableId) = 0;
		virtual Renderable* GetRenderable(int renderableId) = 0;
	};
}