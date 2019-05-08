#include "D3D11ShaderManager.h"

D3D11ShaderManager::D3D11ShaderManager()
{
	std::vector<std::string>* shaders;

	shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/");
	for (auto& it : *shaders)
	{
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
		{
			D3D11VertexShader* vShader = new D3D11VertexShader("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/" + it);
			D3D11PixelShader * pShader = new D3D11PixelShader("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/" + it);

			this->shaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3D11Shader(vShader, pShader);
		}
	}
}

D3D11ShaderManager::~D3D11ShaderManager()
{
}
