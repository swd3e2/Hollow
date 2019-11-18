#include "GLTFImporter.h"

namespace Hollow {
	s_ptr<Import::Model> GLTFImporter::import(const char* filename)
	{
		meshCounter = nodeCounter = animationNodeCounter = 0;

		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF loader;

		if (!loader.LoadASCIIFromFile(&gltfModel, nullptr, nullptr, filename)) {
			HW_ERROR("Cant open file, {}", filename);
			return nullptr;
		}

		Import::Model* model = new Import::Model();

		std::string binaryFilePath = getBinaryFileFolder(filename) + gltfModel.buffers[0].uri;
		std::ifstream binary(binaryFilePath, std::fstream::in | std::fstream::binary);

		if (!binary.is_open()) {
			HW_ERROR("GTLF bin file not found, {}", binaryFilePath.c_str());
			delete model;
			return nullptr;
		}

		tinygltf::Node& modelRootNode = gltfModel.nodes[gltfModel.scenes[0].nodes[0]];
		std::vector<int> meshesToLoad;
		model->rootNode = std::make_shared<Import::Node>();
		model->rootNode->id = 0;
		model->rootNode->jointId = -1;

		processHierarchy(model->rootNode, modelRootNode, model, gltfModel, meshesToLoad);
		processSkin(model, gltfModel, binary);
		processMeshes(model, gltfModel, binary, meshesToLoad);
		processAnimations(model, gltfModel, binary);

		binary.close();

		for (auto& it : model->meshes) {
			for (int i = 0; i < it->indices.size(); i += 3) {
				Vertex& first	= it->vertices[it->indices[i + 0]];
				Vertex& second	= it->vertices[it->indices[i + 1]];
				Vertex& third	= it->vertices[it->indices[i + 2]];

				Vector3 edge1 = second.pos - first.pos;
				Vector3 edge2 = third.pos - first.pos;

				Vector2 uv1 = second.texCoord - first.texCoord;
				Vector2 uv2 = third.texCoord - first.texCoord;

				float f = 1.0f / uv1.x * uv2.y - uv2.x * uv1.y;

				Vector3 tangent(
					f * (uv2.y * edge1.x - uv1.y * edge2.x),
					f * (uv2.y * edge1.y - uv1.y * edge2.y),
					f * (uv2.y * edge1.z - uv1.y * edge2.z)
				);

				Vector3 bitangent(
					f * (uv1.x * edge2.x - uv2.x * edge1.x),
					f * (uv1.x * edge2.y - uv2.x * edge1.y),
					f * (uv1.x * edge2.z - uv2.x * edge1.z)
				);

				first.tangent	+= tangent;
				second.tangent	+= tangent;
				third.tangent	+= tangent;

				first.bitangent	 += bitangent;
				second.bitangent += bitangent;
				third.bitangent	 += bitangent;
			}

			for (auto& vertex : it->vertices) {
				vertex.bitangent = Vector3::normalize(vertex.bitangent);
				vertex.tangent = Vector3::normalize(vertex.tangent);
			}
		}

		return s_ptr<Import::Model>(model);
	}

	void GLTFImporter::processMeshes(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary, std::vector<int>& meshesToLoad)
	{
		for (int i = 0; i < gltfModel.meshes.size(); i++) {
			const tinygltf::Mesh& gltfMesh = gltfModel.meshes[i];

			if (std::find(meshesToLoad.begin(), meshesToLoad.end(), i) == meshesToLoad.end()) continue;

			for (auto& primitive : gltfMesh.primitives) {
				s_ptr<Import::Mesh> mesh = std::make_shared<Import::Mesh>();
				mesh->material = primitive.material;
				mesh->name = gltfMesh.name.size() ? gltfMesh.name : ("Mesh " + std::to_string(meshCounter));
				mesh->id = meshCounter++;

				for (auto& attribute : primitive.attributes) {
					const tinygltf::Accessor& accessor = gltfModel.accessors[attribute.second];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					binary.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

					if (mesh->vertices.size() < accessor.count) {
						mesh->vertices.resize(accessor.count);
					}

					if (accessor.componentType == COMPONENT_TYPE_FLOAT) {
						if (attribute.first == "NORMAL") {
							float* data = new float[accessor.count * 3];
							binary.read((char*)data, sizeof(float) * accessor.count * 3);

							for (int i = 0; i < accessor.count; i++) {
								mesh->vertices[i].normal.x = data[i * 3];
								mesh->vertices[i].normal.y = data[i * 3 + 1];
								mesh->vertices[i].normal.z = data[i * 3 + 2];
							}
							delete[] data;
						} else if (attribute.first == "POSITION") {
							float* data = new float[accessor.count * 3];
							binary.read((char*)data, sizeof(float) * accessor.count * 3);

							for (int i = 0; i < accessor.count; i++) {
								mesh->vertices[i].pos.x = data[i * 3];
								mesh->vertices[i].pos.y = data[i * 3 + 1];
								mesh->vertices[i].pos.z = data[i * 3 + 2];

								if (mesh->vertices[i].pos.x < model->A.x) {
									model->A.x = mesh->vertices[i].pos.x;
								}
								if (mesh->vertices[i].pos.y < model->A.y) {
									model->A.y = mesh->vertices[i].pos.y;
								}
								if (mesh->vertices[i].pos.z < model->A.z) {
									model->A.z = mesh->vertices[i].pos.z;
								}

								if (mesh->vertices[i].pos.x > model->B.x) {
									model->B.x = mesh->vertices[i].pos.x;
								}
								if (mesh->vertices[i].pos.y > model->B.y) {
									model->B.y = mesh->vertices[i].pos.y;
								}
								if (mesh->vertices[i].pos.z > model->B.z) {
									model->B.z = mesh->vertices[i].pos.z;
								}
							}
							delete[] data;
						} else if (attribute.first == "WEIGHTS_0") {
							float* data = new float[accessor.count * 4];
							binary.read((char*)data, sizeof(float) * accessor.count * 4);

							for (int i = 0; i < accessor.count; i++) {
								mesh->vertices[i].boneData.weights[0] = data[i * 4];
								mesh->vertices[i].boneData.weights[1] = data[i * 4 + 1];
								mesh->vertices[i].boneData.weights[2] = data[i * 4 + 2];
								mesh->vertices[i].boneData.weights[3] = data[i * 4 + 3];
							}
							delete[] data;
						} else if (attribute.first == "TEXCOORD_0") {
							float* data = new float[accessor.count * 2];
							binary.read((char*)data, sizeof(float) * accessor.count * 2);

							for (int i = 0; i < accessor.count; i++) {
								mesh->vertices[i].texCoord.x = data[i * 2];
								mesh->vertices[i].texCoord.y = data[i * 2 + 1];
							}
							delete[] data;
						}
					} else if (accessor.componentType == COMPONENT_TYPE_UNSIGNED_SHORT) {
						if (attribute.first == "JOINTS_0") {
							unsigned short* data = new unsigned short[accessor.count * 4];
							binary.read((char*)data, sizeof(unsigned short) * accessor.count * 4);

							for (int i = 0; i < accessor.count; i++) {
								mesh->vertices[i].boneData.joints[0] = data[i * 4 + 0];
								mesh->vertices[i].boneData.joints[1] = data[i * 4 + 1];
								mesh->vertices[i].boneData.joints[2] = data[i * 4 + 2];
								mesh->vertices[i].boneData.joints[3] = data[i * 4 + 3];
							}
							
							delete[] data;
						}
					}
				}

				// Indices
				const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.indices];
				const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
				binary.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

				if (accessor.componentType == COMPONENT_TYPE_UNSIGNED_SHORT) {
					unsigned short* data = new unsigned short[accessor.count];
					binary.read((char*)data, sizeof(unsigned short) * accessor.count);

					for (int i = 0; i < accessor.count; i++) {
						mesh->indices.push_back(data[i]);
					}
					delete[] data;
				} else if (accessor.componentType == COMPONENT_TYPE_UNSIGNED_INT) {
					unsigned int* data = new unsigned int[accessor.count];
					binary.read((char*)data, sizeof(unsigned int) * accessor.count);

					for (int i = 0; i < accessor.count; i++) {
						mesh->indices.push_back(data[i]);
					}
					delete[] data;
				}

				model->meshes.push_back(mesh);

				if (primitive.material != -1) {
					const tinygltf::Material& gltfMaterial = gltfModel.materials[primitive.material];

					if (model->materials.find(primitive.material) == model->materials.end()) {
						Import::Material material;

						material.id = primitive.material;
						material.name = gltfMaterial.name;

						for (auto& value : gltfMaterial.values) {
							if (value.first == "baseColorTexture") {
								material.diffuseTexture = gltfModel.images[gltfModel.textures[value.second.TextureIndex()].source].uri;
							}
							else if (value.first == "metallicRoughnessTexture") {
								material.roughnesTexture = gltfModel.images[gltfModel.textures[value.second.TextureIndex()].source].uri;
							}
							else if (value.first == "metallicFactor") {
								material.metallicFactor = value.second.number_value;
							}
							else if (value.first == "roughnessFactor") {
								material.roughnessFactor = value.second.number_value;
							}
							else if (value.first == "baseColorFactor") {
								material.baseColorFactor = Vector4(
									value.second.number_array[0],
									value.second.number_array[1],
									value.second.number_array[2],
									value.second.number_array[3]
								);
							}
						}

						for (auto& value : gltfMaterial.additionalValues) {
							if (value.first == "emisiveTexture") {
								material.emisiveTexture = gltfModel.images[gltfModel.textures[value.second.TextureIndex()].source].uri;
							}
							else if (value.first == "normalTexture") {
								material.normalTexture = gltfModel.images[gltfModel.textures[value.second.TextureIndex()].source].uri;
							}
							else if (value.first == "occlusionTexture") {
								material.occlusionTexture = gltfModel.images[gltfModel.textures[value.second.TextureIndex()].source].uri;
							}
							else if (value.first == "emissiveFactor") {
								material.emissiveFactor = value.second.number_value;
							}
						}

						for (auto& it : gltfMaterial.extensions) {
							std::vector<std::string> keys = it.second.Keys();
							for (std::string& key : keys) {
								auto& value = it.second.Get(key);
								if (value.IsObject()) {
									std::vector<std::string> innerKeys = value.Keys();
									for (std::string& innerKey : innerKeys) {
										if (key == "diffuseTexture" && innerKey == "index") {
											tinygltf::Value index = value.Get(innerKey);
											int val = index.Get<int>();
											material.diffuseTexture = gltfModel.images[gltfModel.textures[val].source].uri;
										}
										else if (key == "specularGlossinessTexture" && innerKey == "index") {
											tinygltf::Value index = value.Get(innerKey);
											int val = index.Get<int>();
											material.specularTexture = gltfModel.images[gltfModel.textures[val].source].uri;
										}
									}
								}
							}
						}
						model->materials[material.id] = material;
					}
				}
			}
		}
	}

	void GLTFImporter::processSkin(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary)
	{
		if (gltfModel.skins.size() > 0) {
			model->skinned = true;

			const tinygltf::Node& rootNode = gltfModel.nodes[gltfModel.skins[0].skeleton];
			
			model->rootJoint = std::make_shared<Import::Joint>();
			model->rootJoint->id = gltfModel.skins[0].skeleton;
			model->rootJoint->jointId = getJointByNode(gltfModel.skins[0].skeleton, gltfModel);

			const tinygltf::Accessor& accessor = gltfModel.accessors[gltfModel.skins[0].inverseBindMatrices];
			const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
			binary.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

			Matrix4* matrixData = new Matrix4[gltfModel.skins[0].joints.size()];
			binary.read((char*)matrixData, sizeof(Matrix4) * accessor.count);

			processAnimationNode(model->rootJoint, rootNode, model->joints, gltfModel);

			for (int i = 0; i < gltfModel.skins[0].joints.size(); i++) {
				model->joints[gltfModel.skins[0].joints[i]]->inverseBindMatrix = matrixData[i].transpose();
			}

			delete[] matrixData;
		}
	}

	void GLTFImporter::processAnimationNode(s_ptr<Import::Joint>& joint, const tinygltf::Node& modelNode, std::unordered_map<int, s_ptr<Import::Joint>>& joints, const tinygltf::Model& gltfModel)
	{
		joint->name = modelNode.name;
		getTransformMatrix(modelNode, joint);
		joints[joint->id] = joint;

		for (int childId : modelNode.children) {
			s_ptr<Import::Joint> childJoint = std::make_shared<Import::Joint>();
			childJoint->id = childId;
			childJoint->jointId = getJointByNode(childId, gltfModel);

			joint->childs.push_back(childJoint.get());
			processAnimationNode(childJoint, gltfModel.nodes[childId], joints, gltfModel);
		}
	}

	/**
	 * Loads keyframes for animation
	 */
	void GLTFImporter::processAnimations(Import::Model* model, tinygltf::Model& gltfModel, std::ifstream& binary)
	{
		for (auto& animation : gltfModel.animations) {
			s_ptr<Import::Animation> mAnimation = std::make_shared<Import::Animation>();
			mAnimation->name = animation.name;
			
			for (tinygltf::AnimationChannel& channel : animation.channels) {
				tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];
				tinygltf::Accessor& timeAccessor = gltfModel.accessors[sampler.input];
				tinygltf::BufferView& timeBufferView = gltfModel.bufferViews[timeAccessor.bufferView];
				binary.seekg(timeBufferView.byteOffset + timeAccessor.byteOffset, std::fstream::beg);

				float* timeData = new float[timeAccessor.count];
				binary.read((char*)timeData, sizeof(float) * timeAccessor.count);

				if (timeData[timeAccessor.count - 1] > mAnimation->duration) {
					mAnimation->duration = timeData[timeAccessor.count - 1];
				}

				tinygltf::Accessor& valueAccessor = gltfModel.accessors[sampler.output];
				tinygltf::BufferView& valueBufferView = gltfModel.bufferViews[valueAccessor.bufferView];
				binary.seekg(valueBufferView.byteOffset + valueAccessor.byteOffset, std::fstream::beg);

				float* valueData = nullptr;
				if (channel.target_path == "rotation") {
					valueData = new float[valueAccessor.count * 4];
					binary.read((char*)valueData, sizeof(float) * valueAccessor.count * 4);
				} else {
					valueData = new float[valueAccessor.count * 3];
					binary.read((char*)valueData, sizeof(float) * valueAccessor.count * 3);
				}

				s_ptr<Import::AnimationData> data;
				
				if (mAnimation->data.find(channel.target_node) != mAnimation->data.end()) {
					data = mAnimation->data[channel.target_node];
				} else {
					data = std::make_shared<Import::AnimationData>();
					mAnimation->data[channel.target_node] = data;
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

				delete[] timeData;
				delete[] valueData;
			}
			model->animations.push_back(mAnimation);
		}
	}

	/**
	 * Creates hierarchy of nodes and get's their transformations
	 */
	void GLTFImporter::processHierarchy(s_ptr<Import::Node>& node, const tinygltf::Node& modelNode, Import::Model* model, const tinygltf::Model& gltfModel, std::vector<int>& meshesToLoad)
	{
		node->name = modelNode.name.size() ? modelNode.name : ("Node " + std::to_string(nodeCounter++));
		node->meshId = modelNode.mesh;
		getTransformMatrix(modelNode, node);

		if (modelNode.mesh >= 0) {
			meshesToLoad.push_back(modelNode.mesh);
		}

		for (int childId : modelNode.children) {
			s_ptr<Import::Node> childNode = std::make_shared<Import::Node>();
			childNode->id = childId;
			childNode->jointId = getJointByNode(childId, gltfModel);

			node->childs.push_back(childNode.get());
			model->nodes[childId] = childNode;

			processHierarchy(childNode, gltfModel.nodes[childId], model, gltfModel, meshesToLoad);
		}
	}

	int GLTFImporter::getSkinByMesh(int mesh, const tinygltf::Node& node, const tinygltf::Model& model)
	{
		if (node.mesh == mesh) {
			return node.skin;
		}

		if (node.children.size() == 0) {
			return -1;
		} else {
			int result = -1;

			for (auto it : node.children) {
				result = getSkinByMesh(mesh, model.nodes[it], model);
				if (result >= 0) {
					return result;
				}
			}
			return -1;
		}
	}

	std::string GLTFImporter::getBinaryFileFolder(std::string gltfFilePath)
	{
		std::replace(gltfFilePath.begin(), gltfFilePath.end(), '\\', '/');
		gltfFilePath = Helper::trimToLastLineEntry(gltfFilePath.c_str(), '/');
		return gltfFilePath;
	}

	int GLTFImporter::getJointByNode(int nodeId, const tinygltf::Model& gltfModel)
	{
		if (gltfModel.skins.size() == 0) {
			return -1;
		}

		const std::vector<int>& joints = gltfModel.skins[0].joints;

		int lastJointId = 0;
		for (int i = 0; i < joints.size(); i++) {
			if (joints[i] == nodeId) return i;
		}

		return -1;
	}

	int GLTFImporter::getNodeByJoint(int jointId, const tinygltf::Model& gltfModel)
	{
		const std::vector<int>& joints = gltfModel.skins[0].joints;

		if (jointId < joints.size()) {
			return joints[jointId];
		}

		return -1;
	}

	void GLTFImporter::getTransformMatrix(const tinygltf::Node& modelNode, const s_ptr<Import::BaseNode>& node)
	{
		Matrix4 transform = Matrix4::identity();

		if (modelNode.matrix.data()) {
			transform = Matrix4(modelNode.matrix[0],  modelNode.matrix[1],  modelNode.matrix[2],  modelNode.matrix[3],
								modelNode.matrix[4],  modelNode.matrix[5],  modelNode.matrix[6],  modelNode.matrix[7],
								modelNode.matrix[8],  modelNode.matrix[9],  modelNode.matrix[10], modelNode.matrix[11],
								modelNode.matrix[12], modelNode.matrix[13], modelNode.matrix[14], modelNode.matrix[15]).transpose();
		} else {
			Matrix4 rotation = Matrix4::identity();
			Matrix4 scale = Matrix4::identity();
			Matrix4 translation = Matrix4::identity();

			if (modelNode.translation.size() > 0) {
				node->translation = Vector3(modelNode.translation[0], modelNode.translation[1], modelNode.translation[2]);
				translation = Matrix4::translation(node->translation);
			}

			if (modelNode.rotation.size() > 0) {
				node->rotation = Quaternion(modelNode.rotation[0], modelNode.rotation[1], modelNode.rotation[2], modelNode.rotation[3]);
				rotation = Matrix4::rotation(node->rotation);
			}

			if (modelNode.scale.size() > 0) {
				node->scale = Vector3(modelNode.scale[0], modelNode.scale[1], modelNode.scale[2]);
				scale = Matrix4::scaling(node->scale);
			}

			transform = translation * rotation * scale;
		}

		node->transform =  transform;
	}

	bool GLTFImporter::hasMesh(const tinygltf::Node& node, const tinygltf::Model& model)
	{
		if (node.mesh >= 0) {
			return true;
		}

		if (node.children.size() == 0) {
			return false;
		}

		for (auto& child : node.children) {
			if (hasMesh(model.nodes[child], model)) {
				return true;
			}
		}
		return false;
	}
}