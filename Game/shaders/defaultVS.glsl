#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

out VS_OUT {
    vec2 texCoord;
} vout;

layout(std140, binding = 0) uniform WorldViewProjection
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
};

layout(std140, binding = 1) uniform RenderData
{
    mat4 TransformMatrix;
    bool hasAmbientTexture;
};

void main()
{
    vout.texCoord = texCoord;
    gl_Position = vec4(position, 1.0f) * TransformMatrix * ViewMatrix * ProjectionMatrix;
}