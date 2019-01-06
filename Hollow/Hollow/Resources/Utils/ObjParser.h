#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
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
	};

	class HOLLOW_API ObjParser
	{
	private:
		RawMeshData* currentObject;
	public:
		MeshData* LoadObj(const char * filename)
		{
			MeshData* data = new MeshData();

			std::ifstream filestream(filename);

			if (!filestream)
				Hollow::Log::GetCoreLogger()->error("ObjParser: can't open filestream, filename {}", filename);

			std::string linebuff;
			int lines = 0;

			while (filestream.peek() != -1)
			{
				safeGetline(filestream, linebuff);	
				lines++;

				// Trim newline '\r\n' or '\n'
				if (linebuff.size() > 0) {
					if (linebuff[linebuff.size() - 1] == '\n')
						linebuff.erase(linebuff.size() - 1);
				}
				if (linebuff.size() > 0) {
					if (linebuff[linebuff.size() - 1] == '\r')
						linebuff.erase(linebuff.size() - 1);
				}
				// Skip if empty line.
				if (linebuff.empty()) {
					continue;
				}

				// Skip leading space.
				const char *token = linebuff.c_str();
				token += strspn(token, " \t");

				if (token[0] == '#' || token[0] == '\0') continue;

				if (token[0] == 'v' && token[1] == ' ') {
					token += 2;
					parseVertices(data, token);
				}
				if (token[0] == 'v' && token[1] == 'n' && token[2] == ' ') {
					token += 3;
					parseNormals(data, token);
				}
				if (token[0] == 'v' && token[1] == 't' && token[2] == ' ') {
					token += 3;
					parseTexCoords(data, token);
				}
				if (token[0] == 'f' && token[1] == ' ') {
					token += 2;
					parseFace(currentObject, token);
				}
				if (token[0] == 'o' && token[1] == ' ') {
					token += 2;
					if (currentObject != nullptr)
						data->objects.push_back(currentObject);
					
					currentObject = new RawMeshData();
					currentObject->object_name = token;
				}
			}
			if (currentObject != nullptr)
				data->objects.push_back(currentObject);

			Hollow::Log::GetCoreLogger()->warn("objects {}", data->objects.size());

			return data;
		}

	private:
		void parseVertices(MeshData * data, const char* token)
		{
			char* nextToken;
			data->vertices.push_back(atof(strtok_s((char*)token, " \t\r", &nextToken)));
			data->vertices.push_back(atof(strtok_s(nextToken,	 " \t\r", &nextToken)));
			data->vertices.push_back(atof(strtok_s(nextToken,	 " \t\r", &nextToken)));
		}

		void parseTexCoords(MeshData * data, const char* token)
		{
			char* nextToken;
			data->tex_coords.push_back(atof(strtok_s((char*)token, " \t\r", &nextToken)));
			data->tex_coords.push_back(atof(strtok_s(nextToken,    " \t\r", &nextToken)));
		}

		void parseNormals(MeshData * data, const char* token)
		{
			char* nextToken;
			data->normals.push_back(atof(strtok_s((char*)token, " \t\r", &nextToken)));
			data->normals.push_back(atof(strtok_s(nextToken,	" \t\r", &nextToken)));
			data->normals.push_back(atof(strtok_s(nextToken,	" \t\r", &nextToken)));
		}

		void parseFace(RawMeshData* mesh, const char* token)
		{
			char* nextFace;
			char* temp_str;
			char* temp_face;

			while ((temp_face = strtok_s((char*)token, WHITESPACE, (char**)&token)) != NULL) {
				Face face;
				face.vertice_index = atoi(temp_face);

				if (contains(temp_face, "//")) {
					temp_str = (char*)strchr(temp_face, '/');
					temp_str++;
					face.normal_index = atoi(++temp_str);
				} else if (contains(temp_face, "/")) {
					temp_str = (char*)strchr(temp_face, '/');
					face.tex_coord_index = atoi(++temp_str);

					if (contains(temp_str, "/"))
					{
						temp_str = strchr(temp_str, '/');
						face.normal_index = atoi(++temp_str);
					}
				}
				mesh->indices.push_back(face);
			}
		}

		char contains(const char *haystack, const char *needle)
		{
			if (strstr(haystack, needle) == NULL)
				return 0;
			return 1;
		}

		std::istream& safeGetline(std::istream& is, std::string& t)
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