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

	Hollow::Window* window = core.windowManager.Initialize(160, 120, Hollow::WindowType::Bordered);
	Hollow::RenderApi* renderer = core.renderApiManager.Initialize(160, 120);

	Hollow::INPUT_LAYOUT_DESC layoutDesc = {
		{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" }, // pos
		{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }, // texcoord
		{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL" }, // normal
		{ Hollow::INPUT_DATA_TYPE::Float3, "TANGENT" }, // tangent
		{ Hollow::INPUT_DATA_TYPE::Float3, "BITANGENT" }, // bitangent 
	};

	Hollow::InputLayout* layout = Hollow::InputLayout::create(layoutDesc);
	Hollow::Shader* vertexShader = Hollow::Shader::create({ 
		Hollow::SHADER_TYPE::VERTEX, 
		Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/vertex/default.hlsl"), 
		"main" 
	});
	Hollow::Shader* pixelShader = Hollow::Shader::create({
		Hollow::SHADER_TYPE::PIXEL,
		Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/pixel/default.hlsl"),
		"main"
	});

	Hollow::PIPELINE_STATE_DESC pdesc;
	pdesc.vertexShader = vertexShader;
	pdesc.pixelShader = pixelShader;

	Hollow::PipelineState* pipeline = Hollow::PipelineState::create(pdesc);
	std::cin.get();

	return 0;
}