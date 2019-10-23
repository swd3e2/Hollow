#include "GLTFImporter.h"

namespace Hollow {
	void GLTFImporter::processSkin(GLTF::LoadedModel& lModel, tinygltf::Model& model, std::ifstream& file)
	{
		if (model.skins.size() > 0) {
			tinygltf::Node& rootNode = model.nodes[model.skins[0].skeleton];
			lModel.animationRootNode = new Import::AnimationNode();
			lModel.animationRootNode->id = model.skins[0].skeleton;
			lModel.animationRootNode->name = model.nodes[model.skins[0].skeleton].name;
			lModel.animationNodes[lModel.animationRootNode->id] = lModel.animationRootNode;

			tinygltf::Accessor& accessor = model.accessors[model.skins[0].inverseBindMatrices];
			tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			file.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

			Matrix4* matrixData = new Matrix4[model.skins[0].joints.size()];
			file.read((char*)matrixData, sizeof(Matrix4) * accessor.count);

			processAnimationNode(lModel.animationRootNode, rootNode, lModel, model);

			for (int i = 0; i < model.skins[0].joints.size(); i++) {
				lModel.nodes[model.skins[0].joints[i]]->skinned = true;
				lModel.animationNodes[model.skins[0].joints[i]]->localTransform = Matrix4::transpose(matrixData[i]) * lModel.nodes[model.skins[0].joints[i]]->transformation;
			}

			delete[] matrixData;
		}
	}

	int GLTFImporter::getNodeId(const tinygltf::Node& modelNode, tinygltf::Model& tModel)
	{
		for (int i = 0; i < tModel.nodes.size(); i++) {
			if (modelNode == tModel.nodes[i]) {
				return i;
			}
		}
		return -1;
	}

	void GLTFImporter::processAnimationNode(Import::AnimationNode* node, const tinygltf::Node& modelNode, GLTF::LoadedModel& lModel, tinygltf::Model& model)
	{
		for (int childId : modelNode.children) {
			tinygltf::Node& modelAnimationNode = model.nodes[childId];
			
			Import::AnimationNode* animationNode = new Import::AnimationNode();
			animationNode->id = childId;
			animationNode->name = modelAnimationNode.name;

			node->childrens.push_back(animationNode);
			lModel.animationNodes[animationNode->id] = animationNode;
			processAnimationNode(animationNode, modelAnimationNode, lModel, model);
		}
	}

	void GLTFImporter::processAnimation(GLTF::LoadedModel& lModel, tinygltf::Model& model, std::ifstream& file)
	{
		// animation
		for (auto& animation : model.animations) {
			Import::Animation* mAnimation = new Import::Animation();

			for (tinygltf::AnimationChannel& channel : animation.channels) {
				tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];
				tinygltf::Accessor& timeAccessor = model.accessors[sampler.input];
				tinygltf::BufferView& timeBufferView = model.bufferViews[timeAccessor.bufferView];
				file.seekg(timeBufferView.byteOffset + timeAccessor.byteOffset, std::fstream::beg);

				float* timeData = new float[timeAccessor.count];
				file.read((char*)timeData, sizeof(float) * timeAccessor.count);

				tinygltf::Accessor& valueAccessor = model.accessors[sampler.output];
				tinygltf::BufferView& valueBufferView = model.bufferViews[valueAccessor.bufferView];
				file.seekg(valueBufferView.byteOffset + valueAccessor.byteOffset, std::fstream::beg);

				float* valueData = nullptr;
				if (channel.target_path == "rotation") {
					valueData = new float[valueAccessor.count * 4];
					file.read((char*)valueData, sizeof(float) * valueAccessor.count * 4);
				} else {
					valueData = new float[valueAccessor.count * 3];
					file.read((char*)valueData, sizeof(float) * valueAccessor.count * 3);
				}

				Import::AnimationNodeData* data;

				if (mAnimation->data.find(channel.target_node) != mAnimation->data.end()) {
					data = mAnimation->data[channel.target_node];
				} else {
					mAnimation->data[channel.target_node] = data = new Import::AnimationNodeData();
				}

				if (channel.target_path == "rotation") {
					for (int i = 0; i < timeAccessor.count; i++) {
						data->rotations[timeData[i]] = Quaternion(valueData[i * 4], valueData[i * 4 + 1], valueData[i * 4 + 2], valueData[i * 4 + 3]);
					}
				} else if (channel.target_path == "translation") {
					for (int i = 0; i < timeAccessor.count; i++) {
						data->positions[timeData[i]] = Vector3(valueData[i * 3], valueData[i * 3 + 1], valueData[i * 3 + 2]);
					}
				} else if (channel.target_path == "scale") {
					for (int i = 0; i < timeAccessor.count; i++) {
						data->scale[timeData[i]] = Vector3(valueData[i * 3], valueData[i * 3 + 1], valueData[i * 3 + 2]);
					}
				}
			}

			lModel.animations.push_back(mAnimation);
		}
	}

	// Loads node hierarchy and meshes
	void GLTFImporter::load(GLTF::Node* node, const tinygltf::Node& modelNode, tinygltf::Model& tModel, GLTF::LoadedModel& model, std::ifstream& file)
	{
		if (modelNode.matrix.data()) {
			node->transformation = Matrix4(modelNode.matrix.data(), 16).transpose();
		} else {
			Matrix4 rotation = Matrix4::identity();
			Matrix4 scale = Matrix4::identity();
			Matrix4 translation = Matrix4::identity();

			if (modelNode.translation.size() > 0) {
				translation = Matrix4::translation(Vector3(modelNode.translation[0], modelNode.translation[1], modelNode.translation[2]));
			}

			if (modelNode.rotation.size() > 0) {
				rotation = Quaternion(modelNode.rotation[0], modelNode.rotation[1], modelNode.rotation[2], modelNode.rotation[3]).toMatrix4();
			}

			if (modelNode.scale.size() > 0) {
				scale = Matrix4::scaling(Vector3(modelNode.scale[0], modelNode.scale[1], modelNode.scale[2]));
			}

			node->transformation = (scale * rotation * translation).transpose();
		}

		if (modelNode.mesh >= 0) {
			processMesh(node, modelNode, model, tModel, file);
		}

		for (int childId : modelNode.children) {
			tinygltf::Node& childModelNode = tModel.nodes[childId];

			std::string name = childModelNode.name.size() ? childModelNode.name : ("Node " + std::to_string(model.nodeCounter++));
			GLTF::Node* childNode = new GLTF::Node(name);
			childNode->id = childId;
			model.nodes[childNode->id] = childNode;

			load(childNode, childModelNode, tModel, model, file);
			node->childrens.push_back(childNode);
		}
	}

	void GLTFImporter::processMesh(GLTF::Node* node, tinygltf::Node childModelNode, GLTF::LoadedModel& model, tinygltf::Model& tModel, std::ifstream& file)
	{
		tinygltf::Mesh& tMesh = tModel.meshes[childModelNode.mesh];

		for (auto& primitive : tMesh.primitives) {
			GLTF::LoadedMesh* mesh = new GLTF::LoadedMesh();
			mesh->material = primitive.material;
			mesh->name = tMesh.name.size() ? tMesh.name : ("Mesh " + std::to_string(model.meshCounter));
			mesh->id = model.meshIdCounter++;

			for (std::pair<const std::string, int>& attribute : primitive.attributes) {
				tinygltf::Accessor& accessor = tModel.accessors[attribute.second];
				tinygltf::BufferView& bufferView = tModel.bufferViews[accessor.bufferView];
				file.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

				if (accessor.componentType == COMPONENT_TYPE_FLOAT) {
					if (attribute.first == "NORMAL") {
						float* data = new float[accessor.count * 3];
						file.read((char*)data, sizeof(float) * accessor.count * 3);

						for (int i = 0; i < accessor.count; i++) {
							mesh->normals.push_back(Vector3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]));
						}
						delete[] data;
					}
					else if (attribute.first == "POSITION") {
						float* data = new float[accessor.count * 3];
						file.read((char*)data, sizeof(float) * accessor.count * 3);

						for (int i = 0; i < accessor.count; i++) {
							mesh->positions.push_back(Vector3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]));
						}
						delete[] data;
					}
					else if (attribute.first == "WEIGHTS_0") {
						float* data = new float[accessor.count * 4];
						file.read((char*)data, sizeof(float) * accessor.count * 4);

						for (int i = 0; i < accessor.count; i++) {
							mesh->weights.push_back({ data[i * 4] , data[i * 4 + 1] , data[i * 4 + 2] , data[i * 4 + 3] });
						}
						delete[] data;
					}
					else if (attribute.first == "TEXCOORD_0") {
						float* data = new float[accessor.count * 2];
						file.read((char*)data, sizeof(float) * accessor.count * 2);

						for (int i = 0; i < accessor.count; i++) {
							mesh->texCoords.push_back(Vector2(data[i * 2], data[i * 2 + 1]));
						}
						delete[] data;
					}
				}
				else if (accessor.componentType == COMPONENT_TYPE_UNSIGNED_SHORT) {
					if (attribute.first == "JOINTS_0") {
						unsigned short* data = new unsigned short[accessor.count * 4];
						file.read((char*)data, sizeof(unsigned short) * accessor.count * 4);

						for (int i = 0; i < accessor.count; i++) {
							mesh->joints.push_back(new unsigned short[4]{
								(unsigned short)tModel.skins[childModelNode.skin].joints[data[i * 4]],
								(unsigned short)tModel.skins[childModelNode.skin].joints[data[i * 4 + 1]],
								(unsigned short)tModel.skins[childModelNode.skin].joints[data[i * 4 + 2]],
								(unsigned short)tModel.skins[childModelNode.skin].joints[data[i * 4 + 3]]
								});
						}
						delete[] data;
					}
				}
			}

			// Indices
			tinygltf::Accessor& accessor = tModel.accessors[primitive.indices];
			tinygltf::BufferView& bufferView = tModel.bufferViews[accessor.bufferView];
			file.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

			if (accessor.componentType == COMPONENT_TYPE_UNSIGNED_SHORT) {
				unsigned short* data = new unsigned short[accessor.count];
				file.read((char*)data, sizeof(unsigned short) * accessor.count);

				for (int i = 0; i < accessor.count; i++) {
					mesh->indices.push_back(data[i]);
				}
				delete[] data;
			}
			else if (accessor.componentType == COMPONENT_TYPE_UNSIGNED_INT) {
				unsigned int* data = new unsigned int[accessor.count];
				file.read((char*)data, sizeof(unsigned int) * accessor.count);

				for (int i = 0; i < accessor.count; i++) {
					mesh->indices.push_back(data[i]);
				}
				delete[] data;
			}

			model.meshes.push_back(u_ptr<GLTF::LoadedMesh>(mesh));

			tinygltf::Material& material = tModel.materials[primitive.material];

			if (model.materials.find(primitive.material) == model.materials.end()) {
				Import::Material lMaterial;

				lMaterial.id = primitive.material;
				lMaterial.name = material.name;

				for (std::pair<const std::string, tinygltf::Parameter>& values : material.values) {
					if (values.first == "baseColorTexture") {
						lMaterial.diffuseTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
					}
					else if (values.first == "metallicRoughnessTexture") {
						lMaterial.roughnesTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
					}
					else if (values.first == "metallicFactor") {
						lMaterial.metallicFactor = values.second.number_value;
					}
					else if (values.first == "roughnessFactor") {
						lMaterial.roughnessFactor = values.second.number_value;
					}
					else if (values.first == "baseColorFactor") {
						lMaterial.baseColorFactor = Vector4(
							values.second.number_array[0],
							values.second.number_array[1],
							values.second.number_array[2],
							values.second.number_array[3]
						);
					}
				}

				for (std::pair<const std::string, tinygltf::Parameter>& values : material.additionalValues) {
					if (values.first == "emisiveTexture") {
						lMaterial.emisiveTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
					}
					else if (values.first == "normalTexture") {
						lMaterial.normalTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
					}
					else if (values.first == "occlusionTexture") {
						lMaterial.occlusionTexture = tModel.images[tModel.textures[values.second.json_double_value["index"]].source].uri;
					}
					else if (values.first == "emissiveFactor") {
						lMaterial.emissiveFactor = values.second.number_value;
					}
				}

				for (std::pair<const std::string, tinygltf::Value>& it : material.extensions) {
					std::vector<std::string> keys = it.second.Keys();
					for (std::string& key : keys) {
						auto& value = it.second.Get(key);
						if (value.IsObject()) {
							std::vector<std::string> innerKeys = value.Keys();
							for (std::string& innerKey : innerKeys) {
								if (key == "diffuseTexture" && innerKey == "index") {
									tinygltf::Value index = value.Get(innerKey);
									int val = index.Get<int>();
									lMaterial.diffuseTexture = tModel.images[tModel.textures[val].source].uri;
								}
								else if (key == "specularGlossinessTexture" && innerKey == "index") {
									tinygltf::Value index = value.Get(innerKey);
									int val = index.Get<int>();
									lMaterial.specularTexture = tModel.images[tModel.textures[val].source].uri;
								}
							}
						}
					}
				}
				model.materials[lMaterial.id] = lMaterial;
			}	
		}

		node->mesh = model.meshes.size() - 1;
	}

	s_ptr<Import::Model> GLTFImporter::import(const char* filename)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;

		if (!loader.LoadASCIIFromFile(&model, nullptr, nullptr, filename)) {
			HW_ERROR("Cant open file, {}", filename);
			return nullptr;
		}

		std::string somestring;
		somestring = Helper::trimToLastLineEntry(filename, '/');
		if (!somestring.size()) {
			somestring = Helper::trimToLastLineEntry(filename, '\\');
		}

		// If bin file not found
		if (!FileSystem::exists(somestring + model.buffers[0].uri)) {
			HW_ERROR("GTLF bin file not found, {}", somestring.c_str());
			return nullptr;
		}

		u_ptr<GLTF::LoadedModel> lModel(new GLTF::LoadedModel());

		tinygltf::Node& modelRootNode = model.nodes[model.scenes[0].nodes[0]];
		lModel->rootNode = new GLTF::Node(modelRootNode.name);

		std::ifstream file(somestring + model.buffers[0].uri, std::fstream::in | std::fstream::binary);

		load(lModel->rootNode, modelRootNode, model, *lModel, file);
		processAnimation(*lModel, model, file);
		processSkin(*lModel, model, file);

		file.close();

		s_ptr<Import::Model> gltfModel(new Import::Model());
		gltfModel->materials = std::move(lModel->materials);
		gltfModel->animations = std::move(lModel->animations);
		gltfModel->rootNode = lModel->animationRootNode;

		for (auto& model : lModel->meshes) {
			Import::Mesh* mesh = new Import::Mesh;
			mesh->material = model->material;
			mesh->name = model->name;
			mesh->id = model->id;

			for (int i = 0; i < model->positions.size(); i++) {
				Vertex vertex;
				vertex.pos = model->positions[i];

				if (model->normals.size() > i) {
					vertex.normal = model->normals[i];
				}

				if (model->texCoords.size() > i) {
					vertex.texCoord = model->texCoords[i];
				}

				if (model->joints.size() > i) {
					vertex.boneData.joints[0] = model->joints[i][0];
					vertex.boneData.joints[1] = model->joints[i][1];
					vertex.boneData.joints[2] = model->joints[i][2];
					vertex.boneData.joints[3] = model->joints[i][3];
						
					delete[] model->joints[i];
				}

				if (model->weights.size() > i) {
					vertex.boneData.weights[0] = model->weights[i].x;
					vertex.boneData.weights[1] = model->weights[i].y;
					vertex.boneData.weights[2] = model->weights[i].z;
					vertex.boneData.weights[3] = model->weights[i].w;
				}

				mesh->vertices.push_back(vertex);
			}

			for (auto& it : model->indices) {
				mesh->indices.push_back(it);
			}

			gltfModel->meshes.push_back(mesh);
		}
		
		// temp for animation
		//prepareModel(lModel->rootNode, Matrix4::identity(), gltfModel);

		return gltfModel;
	}

	void GLTFImporter::prepareModel(GLTF::Node* node, const Matrix4& parentTransform, const s_ptr<Import::Model>& model)
	{
		Matrix4 transform = parentTransform;

		if (node->mesh != -1) {
			for (auto& it : model->meshes[node->mesh]->vertices) {
				it.pos = it.pos * transform;
				it.normal = it.normal * transform;
				Vector3::normalize(it.normal);
			}
		}

		for (auto& it : node->childrens) {
			prepareModel(it, Matrix4::transpose(node->transformation) * parentTransform, model);
		}
	}

	bool GLTFImporter::hasMesh(tinygltf::Node node, tinygltf::Model& model)
	{
		if (node.mesh >= 0) return true;
		if (node.children.size() == 0) return false;

		for (auto& child : node.children) {
			if (hasMesh(model.nodes[child], model)) return true;
		}
		return false;
	}
}