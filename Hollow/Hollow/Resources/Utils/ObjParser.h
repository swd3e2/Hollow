#pragma once
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Resources/Material.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#define WHITESPACE " \t\n\r"

namespace Hollow {
	struct Face
	{
		int vertice_index;
		int tex_coord_index;
		int normal_index;
	};

	struct RawMaterial
	{
		std::string name;
		std::string diffuse_texture;
		std::string normal_texture;
	};

	struct RawMeshData
	{
		Containers::Vector<Face> indices;
		std::string mtl_filename;
		std::string object_name;
		bool has_texture = false;
		std::string material;
	};

	struct MeshData
	{
		Containers::Vector<float> vertices;
		Containers::Vector<float> tex_coords;
		Containers::Vector<float> normals;
		Containers::Vector<RawMeshData*> objects;
		std::unordered_map<std::string, RawMaterial*> hash_materials;
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
		void LoadMtl(MeshData* data, std::string base_dir);

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