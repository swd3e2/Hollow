#version 460

in VS_OUT {
    vec2 texCoord;
} vout;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}