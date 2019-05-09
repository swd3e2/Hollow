#pragma once

#include "Shader.h"

class ShaderProgram
{
private:
	Shader* pixelShader;
	Shader* vertexShader;
	Shader* geometryShader;
	Shader* computeShader;
	Shader* hullShader;
	Shader* domainShader;
public:
	ShaderProgram(Shader* vShader = nullptr, Shader* pShader = nullptr, Shader* gShader = nullptr, Shader* cShader = nullptr) :
		pixelShader(pShader), vertexShader(vShader), geometryShader(gShader), computeShader(cShader)
	{}

	void setVertexShader(Shader* Shader) { vertexShader = Shader; }
	void setPixelShader(Shader* Shader) { pixelShader = Shader; }
	void setGeomertyShader(Shader* Shader) { geometryShader = Shader; }
	void setComputeShader(Shader* Shader) { computeShader = Shader; }
	void setHullShader(Shader* Shader) { hullShader = Shader; }
	void setDomainShader(Shader* Shader) { domainShader = Shader; }

	Shader* getVertexShader() { return vertexShader; }
	Shader* getPixelShader() { return pixelShader; }
	Shader* getGeomertyShader() { return geometryShader; }
	Shader* getComputeShader() { return computeShader; }
	Shader* getHullShader() { return hullShader; }
	Shader* getDomainShader() { return domainShader; }
};