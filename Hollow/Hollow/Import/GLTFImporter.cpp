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

		tinygltf::Node& modelRootNode = gltfModel.nodes[gltfModel.scenes[0].nodes[0]];
		std::unordered_map<int, s_ptr<Node>> nodes;
		s_ptr<Node> rootNode = std::make_shared<Node>();
		rootNode->name = modelRootNode.name;
		rootNode->id = 0;
		nodes[0] = rootNode;

		processHierarchy(rootNode, modelRootNode, gltfModel, nodes);

		std::string binaryFilePath = getBinaryFileFolder(filename) + gltfModel.buffers[0].uri;
		std::ifstream binary(binaryFilePath, std::fstream::in | std::fstream::binary);

		if (!binary.is_open()) {
			HW_ERROR("GTLF bin file not found, {}", binaryFilePath.c_str());
			delete model;
			return nullptr;
		}

		processSkin(model, gltfModel, binary);
		processMeshes(model, gltfModel, binary);
		processAnimations(model, gltfModel, binary);

		binary.close();

		// temp for animation
		fixModel(rootNode, rootNode->transformation, model);

		return s_ptr<Import::Model>(model);
	}

	void GLTFImporter::processMeshes(Import::Model* model, const tinygltf::Model& gltfModel, std::ifstream& binary)
	{
		for (int i = 0; i < gltfModel.meshes.size(); i++) {
			const tinygltf::Mesh& gltfMesh = gltfModel.meshes[i];

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

								if (mesh->vertices[i].pos.x < model->A.x && mesh->vertices[i].pos.y < model->A.y && mesh->vertices[i].pos.z < model->A.z) {
									model->A = mesh->vertices[i].pos;
								} else if (mesh->vertices[i].pos.x > model->B.x&& mesh->vertices[i].pos.y > model->B.y&& mesh->vertices[i].pos.z > model->B.z) {
									model->B = mesh->vertices[i].pos;
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
			const tinygltf::Node& rootNode = gltfModel.nodes[gltfModel.skins[0].skeleton];
			
			model->rootNode = std::make_shared<Import::AnimationNode>();
			model->rootNode->id = gltfModel.skins[0].skeleton;
			model->rootNode->jointId = getJointByNode(gltfModel.skins[0].skeleton, gltfModel);
			model->rootNode->name = gltfModel.nodes[gltfModel.skins[0].skeleton].name;
			model->rootNode->localTransform2 = getTransformMatrix(gltfModel.nodes[gltfModel.skins[0].skeleton]);
			model->nodes[model->rootNode->id] = model->rootNode;

			const tinygltf::Accessor& accessor = gltfModel.accessors[gltfModel.skins[0].inverseBindMatrices];
			const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
			binary.seekg(bufferView.byteOffset + accessor.byteOffset, std::fstream::beg);

			Matrix4* matrixData = new Matrix4[gltfModel.skins[0].joints.size()];
			binary.read((char*)matrixData, sizeof(Matrix4) * accessor.count);

			processAnimationNode(model->rootNode, rootNode, model->nodes, gltfModel);

			for (int i = 0; i < gltfModel.skins[0].joints.size(); i++) {
				model->nodes[gltfModel.skins[0].joints[i]]->localTransform = Matrix4::transpose(matrixData[i]);
			}

			delete[] matrixData;
		}
	}

	void GLTFImporter::processAnimationNode(s_ptr<Import::AnimationNode> node, const tinygltf::Node& modelNode, std::unordered_map<int, s_ptr<Import::AnimationNode>>& nodes, const tinygltf::Model& gltfModel)
	{
		for (int childId : modelNode.children) {
			const tinygltf::Node& modelAnimationNode = gltfModel.nodes[childId];

			s_ptr<Import::AnimationNode> animationNode = std::make_shared<Import::AnimationNode>();
			animationNode->id = childId;
			animationNode->jointId = getJointByNode(childId, gltfModel);
			animationNode->name = modelAnimationNode.name;
			animationNode->localTransform2 = getTransformMatrix(modelAnimationNode);
			
			if (modelAnimationNode.translation.size() > 0) {
				animationNode->translation = Vector3(modelAnimationNode.translation[0], modelAnimationNode.translation[1], modelAnimationNode.translation[2]);
			}

			if (modelAnimationNode.rotation.size() > 0) {
				animationNode->rotation = Quaternion(modelAnimationNode.rotation[0], modelAnimationNode.rotation[1], modelAnimationNode.rotation[2], modelAnimationNode.rotation[3]);
			}

			if (modelAnimationNode.scale.size() > 0) {
				animationNode->scale = Vector3(modelAnimationNode.scale[0], modelAnimationNode.scale[1], modelAnimationNode.scale[2]);
			}

			animationNode->localTransform = Matrix4::identity();

			node->childrens.push_back(animationNode);
			nodes[animationNode->id] = animationNode;
			processAnimationNode(animationNode, modelAnimationNode, nodes, gltfModel);
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

				s_ptr<Import::AnimationNodeData> data;

				if (mAnimation->data.find(channel.target_node) != mAnimation->data.end()) {
					data = mAnimation->data[channel.target_node];
				} else {
					data = std::make_shared<Import::AnimationNodeData>();
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
	void GLTFImporter::processHierarchy(s_ptr<Node>& node, const tinygltf::Node& modelNode, const tinygltf::Model& gltfModel, std::unordered_map<int, s_ptr<Node>>& nodes)
	{
		node->transformation = getTransformMatrix(modelNode);

		for (int childId : modelNode.children) {
			const tinygltf::Node& childModelNode = gltfModel.nodes[childId];

			s_ptr<Node> childNode = std::make_shared<Node>();
			childNode->name = childModelNode.name.size() ? childModelNode.name : ("Node " + std::to_string(nodeCounter++));
			childNode->id = childId;
			childNode->jointId = getJointByNode(childId, gltfModel);
			childNode->mesh = childModelNode.mesh;

			node->childrens.push_back(childNode);
			nodes[childNode->id] = childNode;

			processHierarchy(childNode, childModelNode, gltfModel, nodes);
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
		std::string folderPath;
		folderPath = Helper::trimToLastLineEntry(gltfFilePath.c_str(), '/');
		if (!folderPath.size()) {
			folderPath = Helper::trimToLastLineEntry(gltfFilePath.c_str(), '\\');
		}
		return folderPath;
	}

	int GLTFImporter::getJointByNode(int nodeId, const tinygltf::Model& gltfModel)
	{
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

	Matrix4 GLTFImporter::getTransformMatrix(const tinygltf::Node& modelNode)
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
				translation = Matrix4::translation(modelNode.translation[0], modelNode.translation[1], modelNode.translation[2]);
			}

			if (modelNode.rotation.size() > 0) {
				rotation = Matrix4::rotation(Quaternion(modelNode.rotation[0], modelNode.rotation[1], modelNode.rotation[2], modelNode.rotation[3]));
			}

			if (modelNode.scale.size() > 0) {
				scale = Matrix4::scaling(modelNode.scale[0], modelNode.scale[1], modelNode.scale[2]);
			}

			transform = scale * rotation * translation;
		}

		return transform;
	}

	void GLTFImporter::fixModel(const s_ptr<Node>& node, const Matrix4& parentTransform, Import::Model* model)
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
			fixModel(it, Matrix4::transpose(node->transformation) * parentTransform, model);
		}
	}
}