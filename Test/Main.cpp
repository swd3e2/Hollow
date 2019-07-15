#include "Hollow/Core.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include <iostream>
#include "Hollow/Common/FileSystem.h"

class Transform
{
public:
	int x, y, z;
	Transform(int x, int y, int z) :
		x(x), y(y), z(z)
	{}
};

int main() 
{
	Hollow::Core core(Hollow::RendererType::DirectX);

	Hollow::Window* window = core.windowManager.Initialize(160, 120, Hollow::WindowType::Borderless);
	Hollow::RenderApi* renderer = core.renderApiManager.Initialize(160, 120);

	Hollow::INPUT_LAYOUT_DESC layoutDesc = {
		{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" }, // pos
		{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }, // texcoord
		{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL" }, // normal
		{ Hollow::INPUT_DATA_TYPE::Float3, "TANGENT" }, // tangent
		{ Hollow::INPUT_DATA_TYPE::Float3, "BITANGENT" }, // bitangent 
	};

	Hollow::InputLayout* layout = renderer->CreateLayout(layoutDesc);
	Hollow::ShaderManager::instance()->create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent(""), "main" });
	std::cin.get();

	return 0;
}