#version 460

in VS_OUT {
    vec2 texCoord;
} vout;

out vec4 FragColor;

uniform sampler2D ambient; //0

layout(std140, binding = 1) uniform RenderData
{
    mat4 TransformMatrix;
    bool hasAmbientTexture;
};

void main()
{
    vec4 color;
    
    if (hasAmbientTexture) {
        color = texture(ambient, vout.texCoord);
        if (color.a < 0.15) {
            discard;
        }
    } else {
        color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    }
    
    FragColor = color;
}