#version 330

out vec4 FragColor;

in vec2 outtexCoord;
in vec3 outnormal;
in vec3 outtangent;
in vec3 outbitangent;

void main()
{
	FragColor = vec4(outnormal.x, outnormal.y, outnormal.z, 0.0f);

}