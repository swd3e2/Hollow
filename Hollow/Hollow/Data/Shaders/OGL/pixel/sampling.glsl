#version 460

in VS_OUT
{
	vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D light; //0;

void main()
{
	vec2 texc = vec2(fs_in.texCoord.x, 1.0 - fs_in.texCoord.y);
	
	FragColor = texture(light, texc);
}