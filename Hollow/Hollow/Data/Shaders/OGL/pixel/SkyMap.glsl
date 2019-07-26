#version 460

in VS_OUT
{
	vec3 texCoord;
} fs_in;

out vec4 FragColor;

uniform samplerCube tex4;
uniform sampler2D tex5;

void main()
{
	float depth = texture(tex5, vec2(gl_FragCoord.x / 2560.0f, gl_FragCoord.y / 1440.0f)).r;

	if (depth == 1.0f) {
		FragColor = texture(tex4, fs_in.texCoord);
		return;
	}
	discard;
}