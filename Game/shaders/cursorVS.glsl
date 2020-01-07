#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

out VS_OUT{
    vec2 texCoord;
} vout;

void main()
{
    vout.texCoord = texCoord;
    gl_Position = vec4(position, 1.0f);
}