#include "Material.h"
#include "Hollow/Graphics/TextureManager.h"

Hollow::Material::~Material()
{
	if (diffuseTexture != nullptr) {
		TextureManager::instance()->remove(diffuseTexture);
	}
}
