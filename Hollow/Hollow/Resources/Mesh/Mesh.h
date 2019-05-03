#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "SubMesh.h"
#include "Hollow/Containers/vector.h"

class Mesh
{
private:
	bool ready;
public:
	int mNumSubmeshes;
	Hollow::Vector<SubMesh*> subMeshes;
	std::string filename;
public:
	Mesh() : mNumSubmeshes(0), ready(false) {}

	inline void setReady(bool status) { ready = status; }
	inline bool getReady() { return ready; }
};

#endif