#include "OGLInputLayoutManager.h"

namespace Hollow {
	s_ptr<InputLayout> OGLInputLayoutManager::create(const INPUT_LAYOUT_DESC& desc)
	{
		s_ptr<OGLInputLayout> layout = std::make_shared<OGLInputLayout>();
		layout->layout = desc.layout;
		layout->size = desc.stride;

		glCreateVertexArrays(1, &layout->vao);

		for (int i = 0; i < desc.layout.size(); i++) {
			glEnableVertexArrayAttrib(layout->vao, i);
			GLuint type = OGLHelper::getInputLayoutFormat(desc.layout[i].type);
			
			if (type == GL_FLOAT) {
				glVertexArrayAttribFormat(layout->vao, i, desc.layout[i].getNumberElements(), type, GL_FALSE, desc.layout[i].offset);
			} else if (type == GL_INT || type == GL_BOOL) {
				glVertexArrayAttribIFormat(layout->vao, i, desc.layout[i].getNumberElements(), type, desc.layout[i].offset);
			}

			glVertexArrayAttribBinding(layout->vao, i, 0);
		}

		return std::static_pointer_cast<InputLayout>(layout);
	}
}
