#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Graphics/IMaterial.h"
#include <unordered_map>
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/DirectX/D3DMaterial.h"

class HOLLOW_API MaterialManager : public CModule<MaterialManager>
{
private:
	std::unordered_map<std::string, D3DMaterial*> materialList;
public:
	void StartUp();
	void Shutdown();

	void createMaterial(std::string name);
	std::unordered_map<std::string, D3DMaterial*>* getMaterialList();
};