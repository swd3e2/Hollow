#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
} fs_in;

layout(std140, binding = 8) uniform PerObject
{
	bool horizontal;
};

uniform sampler2D image; //0
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, fs_in.texCoord).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, fs_in.texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, fs_in.texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, fs_in.texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, fs_in.texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }

    FragColor = vec4(result, 1.0f);
}