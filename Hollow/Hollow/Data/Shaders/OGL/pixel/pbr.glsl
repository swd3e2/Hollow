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

const float PI = 3.14159265359;

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

uniform sampler2D diffuseTexture; //0
uniform sampler2D normalTexture; //1
uniform sampler2D positionTexture; //2
uniform sampler2D pbrTexture; //3
uniform sampler2D shadowMap; //4


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
} 

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

	vec3 position = texture(positionTexture, texCoords).rgb;
	vec3 normal = texture(normalTexture, texCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	
	vec4 diffuse = texture(diffuseTexture, texCoords);
    vec4 pbrParams = texture(pbrTexture, texCoords);
    float metallic = pbrParams.r;
    float roughness = pbrParams.g;
    float emissive = pbrParams.b;
    float ao = pbrParams.a;      


    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, vec3(diffuse), metallic);
    // выражение отражающей способности
    vec3 Lo = vec3(0.0);
    vec3 N = normal;
    vec3 V = normalize(cameraPosition - position);
	for (int i = 0; i < numLights; i++) {
        vec3 L = normalize(lights[i].position - position);
        vec3 H = normalize(V + L);

        float distance    = length(lights[i].position - position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lights[i].color * attenuation; 

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
            
        // прибавляем результат к исходящей энергетической яркости Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * vec3(diffuse) / PI + specular) * radiance * NdotL; 
	}

    vec3 ambient = vec3(0.03) * vec3(diffuse) * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1.0);

    //FragColor = texture(pbrTexture, texCoords);
	//FragColor = vec4(normal, 1.0f);
	//FragColor = diffuse;
}