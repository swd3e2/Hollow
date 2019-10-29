#pragma once

#ifndef HW_TEST_CUBE_H
#define HW_TEST_CUBE_H

#include "Resources/Mesh/Mesh.h"
#include "Graphics/Vertex.h"
#include "Graphics/HardwareBufferManager.h"

namespace Hollow {
	std::pair<s_ptr<VertexBuffer>, s_ptr<IndexBuffer>> getCube();
}

#endif