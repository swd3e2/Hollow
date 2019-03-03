#pragma once
#include <string>

class IMaterial
{
public:
	bool active = false;
	std::string name;
	std::string diffuse_texture;
	std::string normal_texture;
};