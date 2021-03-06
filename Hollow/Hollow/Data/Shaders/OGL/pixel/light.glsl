#version 460

out vec4 FragColor;

in VS_OUT
{
	vec2 texCoord;
} fs_in;

uniform float lightTypeAmbient	= 0;
uniform float lightTypeDiffuse	= 1;
uniform float lightTypePoint	= 2;
uniform float lightTypeSpot		= 3;

struct Light
{
	vec3 position;		// 12
	float constant;		// 16
	vec3 direction;		// 28
	float linearAttenuation; // 32
	vec3 color;			// 44
	float quadratic;	// 48
	float cutoff;		// 52
	float distance;		// 56
	int type;			// 60
	int pad;			// 64
};

layout(std140, binding = 0) uniform Matrices
{
	mat4 WVP;
	vec3 cameraPosition;
	vec3 cameraViewDirection;
};

layout(std140, binding = 1) uniform ShadowMatrices
{
	mat4 ShadowWVP;
	vec2 texelSize;
	float bias;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 transform;
	vec3 color;
	bool hasAnimation;
};

layout(std140, binding = 5) uniform LightData
{
	Light[100] lights;
	int numLights;
};

uniform sampler2D tex0; //0
uniform sampler2D tex1; //1
uniform sampler2D tex2; //2
uniform sampler2D tex3; //3

float SampleShadowMap(sampler2D shadowMap, vec2 uv, float compare)
{
	return step(compare, texture(shadowMap, uv).r);
}

float SampleShadowMapLinear(sampler2D shadowMap, vec4 shadowMapCoords)
{
	vec3 ProjCoords = shadowMapCoords.xyz / shadowMapCoords.w;
	vec2 uv;
	uv.x = 0.5f * ProjCoords.x + 0.5f;
	uv.y = 0.5f * ProjCoords.y + 0.5f;
	float z = 0.5f * ProjCoords.z + 0.5f;

	vec2 pixelPos = uv / texelSize + vec2(0.5);
	vec2 fracPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart) * texelSize;

	float blTexel = SampleShadowMap(shadowMap, startTexel, z);
	float brTexel = SampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, 0.0f), z);
	float tlTexel = SampleShadowMap(shadowMap, startTexel + vec2(0.0f, texelSize.y), z);
	float trTexel = SampleShadowMap(shadowMap, startTexel + vec2(texelSize.x, texelSize.y), z);

	float mixA = mix(blTexel, tlTexel, fracPart.y);
	float mixB = mix(brTexel, trTexel, fracPart.y);

	return mix(mixA, mixB, fracPart.x);
	/*if (clamp(uv.x, 0.0f, 1.0f) == uv.x && uv.y == clamp(uv.y, 0.0f, 1.0f)) {
		float shadowDepth = texture(shadowMap, uv).r;
		if (shadowDepth < (z - bias)) {

		}
	}*/
}

vec4 CalcDiffuseLight(Light light, vec3 normal, vec3 fragPos)
{
	vec4 result = clamp(dot(normal, normalize(light.direction)) * vec4(light.color, 1.0f), 0.0f, 1.0f);

	vec3 toCameraVec = reflect(light.direction, normal);
	vec3 cameraToPosVec = -normalize(cameraPosition - fragPos);
	float dotRes = clamp(dot(toCameraVec, cameraToPosVec), 0.0, 1.0);

	if (dotRes > 0.99f) {
		result += vec4(1.0f, 1.0f, 1.0f, 0.0f);
	}

	return result;
}

vec4 CalcPointLight(Light light, vec3 normal, vec3 fragPos)
{
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec3 lightDir = normalize(light.position - fragPos);
	float distance = length(light.position - fragPos);
	vec3 toCameraVec = reflect(lightDir, normal);
	vec3 cameraToPosVec = normalize(fragPos - cameraPosition);

	if (distance < light.distance) {
		float attenuation = 1.0 / (light.constant + light.linearAttenuation * distance + light.quadratic * (distance * distance));
		color += clamp(dot(normal, lightDir), 0.0, 1.0) * vec4(light.color, 0.0f) * attenuation;// *2.0f;
		float result = clamp(dot(toCameraVec, cameraToPosVec), 0.0, 1.0);

		if (result > 0.99f) {
			color += vec4(1.0f, 1.0f, 1.0f, 0.0f) * pow(result, 1);
		}
	}

	return color;
}

void main()
{
	vec2 texCoords = vec2(fs_in.texCoord.x, 1.0f - fs_in.texCoord.y);

	vec3 position = texture(tex2, texCoords).rgb;
	vec3 normal = texture(tex1, texCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	
	vec4 diffuse = texture(tex0, texCoords);

	//vec4 shadowPos = vec4(position, 1.0f) * ShadowWVP;
	//SampleShadowMapLinear(tex3, shadowPos);

	for (int i = 0; i < numLights; i++) {
		if (lights[i].type == lightTypeAmbient) {
			diffuse += vec4(lights[i].color, 0.0f);
		} else if (lights[i].type == lightTypeDiffuse) {
			diffuse += CalcDiffuseLight(lights[i], normal, position);
		} else if (lights[i].type == lightTypePoint) {
			diffuse += CalcPointLight(lights[i], normal, position);
		}
	}

	//FragColor = vec4(normal, 1.0f);
	FragColor = diffuse;
}