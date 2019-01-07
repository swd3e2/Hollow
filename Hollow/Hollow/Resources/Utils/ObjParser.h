#pragma once
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Resources/Material.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string>
#define WHITESPACE " \t\n\r"

namespace Hollow {
	struct Face
	{
		int vertice_index;
		int tex_coord_index;
		int normal_index;
	};

	struct RawMeshData
	{
		Containers::Vector<Face> indices;
		std::string mtl_filename;
		std::string object_name;
	};

	struct MeshData
	{
		Containers::Vector<float> vertices;
		Containers::Vector<float> tex_coords;
		Containers::Vector<float> normals;
		Containers::Vector<RawMeshData*> objects;
		Containers::Vector<Material*> materials;
	};

	class HOLLOW_API ObjParser
	{
	private:
		RawMeshData* currentObject;
	public:
		MeshData* LoadObj(const char * filename, const char* material_base_dir);

	private:
		void parseVertices(MeshData * data, const char* token);
		void parseTexCoords(MeshData * data, const char* token);
		void parseNormals(MeshData * data, const char* token);
		void parseFace(RawMeshData* mesh, const char* token);
		void LoadMtl(std::string base_dir);

		inline char contains(const char *haystack, const char *needle)
		{
			if (strstr(haystack, needle) == NULL)
				return 0;
			return 1;
		}

		inline std::istream& safeGetline(std::istream& is, std::string& t)
		{
			t.clear();

			std::istream::sentry se(is, true);
			std::streambuf* sb = is.rdbuf();

			if (se) {
				for (;;) {
					int c = sb->sbumpc();
					switch(c) {
						case '\n':
							return is;
						case '\r':
							if (sb->sgetc() == '\n') sb->sbumpc();
          					return is;
        				case EOF:
          					// Also handle the case when the last line has no line ending
          					if (t.empty()) is.setstate(std::ios::eofbit);
          					return is;
        				default:
         			 		t += static_cast<char>(c);
					}
				}
			}
			return is;
		}
	};

}