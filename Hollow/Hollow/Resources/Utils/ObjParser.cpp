#include "ObjParser.h"

namespace Hollow {

	MeshData* ObjParser::LoadObj(std::string filename)
	{
		std::string material_base_dir = Helper::trim_to_last_line_entry(filename.c_str(), '/');
		
		if (currentObject != nullptr) {
			currentObject = nullptr;
		}

		MeshData* data = new MeshData();

		std::ifstream filestream(filename.c_str());

		if (!filestream) {
			HW_ERROR("ObjParser: can't open filestream, filename {}", filename);
			return nullptr;
		}

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

			if (token[0] == '#' && token[1] == ' ' && token[2] != 'o' || token[0] == '\0') continue;

			if (token[0] == 'v' && token[1] == ' ') {
				token += 2;
				parseVertices(data, token);
				continue;
			}
			if (token[0] == 'v' && token[1] == 'n' && token[2] == ' ') {
				token += 3;
				parseNormals(data, token);
				continue;
			}
			if (token[0] == 'v' && token[1] == 't' && token[2] == ' ') {
				token += 3;
				if (currentObject != nullptr)
					currentObject->has_texture = true;
					
				parseTexCoords(data, token);
				continue;
			}
			if (token[0] == 'f' && token[1] == ' ') {
				token += 2;
				parseFace(currentObject, token);
				continue;
			}
			// Load all material properties from mtl file
			if ((0 == strncmp(token, "mtllib", 6)) && token[6] == ' ') {
				token += 7;
				std::string base_dir = material_base_dir;
				base_dir += token;
				LoadMtl(data, base_dir);
				continue;
			}

			// Save usage of material in rawmeshdata
			if ((0 == strncmp(token, "usemtl", 6)) && token[6] == ' ') {
				token += 7;
				if (currentObject != nullptr)
					currentObject->material = token;
				continue;
			}
			if (token[0] == 'o' && token[1] == ' ' ||
				token[0] == '#' && token[1] == ' ' && token[2] == 'o' && token[3] == 'b'
				&& token[4] == 'j' && token[5] == 'e' && token[6] == 'c' && token[7] == 't'
				) {
				if (token[0] == 'o' && token[1] == ' ') {
					token += 2;
				} else {
					token += 7;
				}
				// if object has at least one indices we can save it safely
				if (currentObject != nullptr && currentObject->indices.size() != 0) {
					data->objects.push_back(currentObject);
				}

				currentObject = new RawMeshData();
				currentObject->object_name = token;
				continue;
			}
		}

		if (!mtllibParsed) {
			std::string mtl_filepath = filename.substr(0, filename.size() - 4);
			mtl_filepath += ".mtl";
			LoadMtl(data, mtl_filepath);
		}

		if (currentObject != nullptr)
			data->objects.push_back(currentObject);

		return data;
	}

	void ObjParser::parseVertices(MeshData* data, const char * token)
	{
		char* nextToken;
		data->vertices.push_back(atof(strtok_s((char*)token, " \t\r", &nextToken)));
		data->vertices.push_back(atof(strtok_s(nextToken, " \t\r", &nextToken)));
		data->vertices.push_back(atof(strtok_s(nextToken, " \t\r", &nextToken)));
	}

	void ObjParser::parseTexCoords(MeshData* data, const char * token)
	{
		char* nextToken;
		data->tex_coords.push_back(atof(strtok_s((char*)token, " \t\r", &nextToken)));
		data->tex_coords.push_back(atof(strtok_s(nextToken, " \t\r", &nextToken)));
	}

	void ObjParser::parseNormals(MeshData* data, const char * token) 
	{
		char* nextToken;
		data->normals.push_back(atof(strtok_s((char*)token, " \t\r", &nextToken)));
		data->normals.push_back(atof(strtok_s(nextToken, " \t\r", &nextToken)));
		data->normals.push_back(atof(strtok_s(nextToken, " \t\r", &nextToken)));
	}

	void ObjParser::parseFace(RawMeshData* mesh, const char * token)
	{
		char* nextFace;
		char* temp_str;
		char* temp_face;
		int vertex_count = 0;

		while ((temp_face = strtok_s((char*)token, WHITESPACE, (char**)&token)) != NULL) {
			vertex_count++;

			Face face;
			face.vertice_index = atoi(temp_face) - 1;

			if (contains(temp_face, "//")) {
				temp_str = (char*)strchr(temp_face, '/');
				temp_str++;
				face.normal_index = atoi(++temp_str) - 1;
			}
			else if (contains(temp_face, "/")) {
				temp_str = (char*)strchr(temp_face, '/');
				face.tex_coord_index = atoi(++temp_str) - 1;

				if (contains(temp_str, "/"))
				{
					temp_str = strchr(temp_str, '/');
					face.normal_index = atoi(++temp_str) - 1;
				}
			}
			// Triangulate faces
			if (vertex_count > 3) {
				mesh->indices.push_back(mesh->indices[mesh->indices.size() - 1]);
				mesh->indices.push_back(face);
				mesh->indices.push_back(mesh->indices[mesh->indices.size() - 5]);
			} else {
				mesh->indices.push_back(face);
			}
		}
	}

	void ObjParser::LoadMtl(MeshData* data, std::string base_dir)
	{
		std::fstream filestream(base_dir);
		if (!filestream) {
			HW_ERROR("ObjParser: can't parse mtl file, error while trying open filestream, filename {}", base_dir);
			return;
		}

		RawMaterial* material = nullptr;

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

			// Parse name
			if ((0 == strncmp(token, "newmtl", 6)) && token[6] == ' ') {
				token += 7;
				if (material != nullptr) {
					data->hash_materials[material->name] = material;
				}
				material = new RawMaterial();
				material->name = token;
			}
			// Parse diffues texture
			if ((0 == strncmp(token, "map_Kd", 6)) && token[6] == ' ') {
				token += 7;
				material->diffuse_texture = token;
			}
			if ((0 == strncmp(token, "map_Ns", 6)) && token[6] == ' ') {
				token += 7;
				material->normal_texture = token;
			}
			if ((0 == strncmp(token, "map_Ks", 6)) && token[6] == ' ') {
				token += 7;
				material->specular_texture = token;
			}
			if ((0 == strncmp(token, "Ns", 2)) && token[2] == ' ') {
				token += 3;
				material->Ns = atof(token);
			}
			if ((0 == strncmp(token, "Ka", 2)) && token[2] == ' ') {
				token += 3;
				char* nextToken;

				material->Ka[0] = atof(strtok_s((char*)token, " \t\r", &nextToken));
				material->Ka[1] = atof(strtok_s(nextToken, " \t\r", &nextToken));
				material->Ka[2] = atof(strtok_s(nextToken, " \t\r", &nextToken));
			}
			if ((0 == strncmp(token, "Kd", 2)) && token[2] == ' ') {
				token += 3;
				char* nextToken;

				material->Kd[0] = atof(strtok_s((char*)token, " \t\r", &nextToken));
				material->Kd[1] = atof(strtok_s(nextToken, " \t\r", &nextToken));
				material->Kd[2] = atof(strtok_s(nextToken, " \t\r", &nextToken));
			}
			if ((0 == strncmp(token, "Ks", 2)) && token[2] == ' ') {
				token += 3;
				char* nextToken;

				material->Ks[0] = atof(strtok_s((char*)token, " \t\r", &nextToken));
				material->Ks[1] = atof(strtok_s(nextToken, " \t\r", &nextToken));
				material->Ks[2] = atof(strtok_s(nextToken, " \t\r", &nextToken));
			}
			if ((0 == strncmp(token, "Ke", 2)) && token[2] == ' ') {
				token += 3;
				char* nextToken;

				material->Ke[0] = atof(strtok_s((char*)token, " \t\r", &nextToken));
				material->Ke[1] = atof(strtok_s(nextToken, " \t\r", &nextToken));
				material->Ke[2] = atof(strtok_s(nextToken, " \t\r", &nextToken));
			}
		}
		data->hash_materials[material->name] = material;
		filestream.close();
		this->mtllibParsed = true;
	}

}