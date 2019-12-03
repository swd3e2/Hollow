#pragma once
#include "Components/RenderableComponent.h"
#include "Hollow/Import/HollowModelExporter.h"
#include "Hollow/Import/Mesh.h"

class ExportHelper
{
public:
	static Hollow::s_ptr<Hollow::Export::Model> make(Hollow::s_ptr<Hollow::Import::Model> imodel)
	{
		using namespace Hollow;
		s_ptr<Export::Model> model = std::make_shared<Export::Model>();

		model->rootJoint = imodel->rootJoint;
		model->rootNode = imodel->rootNode;
		model->joints = imodel->joints;
		model->inverseBindMatrices = imodel->inverseBindMatrices;

		for (auto& it : imodel->materials) {
			Export::Material material;
			material.id = it.id;
			material.name = it.name;
			
			material.diffuseTexture = it.diffuseTexture;
			material.normalTexture = it.normalTexture;
			material.specularTexture = it.specularTexture;
			material.roughnesTexture = it.roughnesTexture;
			// @TODO: add metallic texture
			//material.metallicTexture = it.metallicFactor;

			material.color = it.baseColorFactor;
			material.metallic = it.metallicFactor;
			material.roughness = it.roughnessFactor;


			//material.ambientOcclusion = it;
			//material.emissiveColor = it->materialData.roughnessFactor;
			//material.specular = it->materialData.specular;

			model->materials.push_back(material);
		}

		for (auto& it : imodel->nodes) {
			s_ptr<Export::Node> node = std::make_shared<Export::Node>();

			node->id = it->id;
			node->name = it->name;
			node->joint = it->jointId;
			node->mesh = it->meshId;

			// @TODO: check joints trs
			node->translation = it->translation;
			node->scale = it->scale;
			node->rotation = it->rotation;

			node->childs = it->childs;

			model->nodes.push_back(node);
		}

		for (auto& it : imodel->meshes) {
			s_ptr<Export::Mesh> mesh = std::make_shared<Export::Mesh>();
			mesh->id = it->id;
			
			for (int j = 0; j < it->vertices.size(); j++) {
				mesh->vertices.push_back(it->vertices[j]);
			}
			mesh->indices = it->indices;
			model->meshes.push_back(mesh);
		}

		for (auto& it : imodel->animations) {
			s_ptr<Export::Animation> animation = std::make_shared<Export::Animation>();
			
			animation->duration = it->duration;
			animation->name = it->name;

			for (auto& data : it->data) {
				s_ptr<Export::AnimationData> animationData = std::make_shared<Export::AnimationData>();
				animationData->positions = data.second->positions;
				animationData->rotations = data.second->rotations;
				animationData->scale = data.second->scale;

				animation->data[data.first] = animationData;
			}

			model->animations.push_back(animation);
		}

		return model;
	}
};