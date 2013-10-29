// Fragment Shader – file "mesh_pointlight.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30

// Light vars
uniform vec4 ambLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float ambLightIntensity = 0.15f;

uniform vec4 diffuseLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float diffuseLightIntensity = 1.0f;
//uniform float cAttenuation = 1.0f;
//uniform float lAttenuation = 0.08f;
//uniform float qAttenuation = 0.01f;
uniform float radius = 1.0f;
uniform float falloff = 2.0f;

uniform float specularHardness = 32.0f;
uniform vec4 specularLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float specularLightIntensity = 1.0f;

// Texture vars
uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform int diffuseTexEnabled;
uniform int ambientTexEnabled;

in  vec3 ex_Color;
in  vec3 ex_Normal;
in  vec2 ex_UV;
in  vec3 ex_LightDirW;
in  vec3 ex_ViewDirW;

out vec4 out_Color;
 
void main(void)
{
	vec4 ambientLight, diffuseLight, specularLight;
	
	specularLight = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	vec3 viewDirW = normalize(ex_ViewDirW);
	vec3 normal = normalize(ex_Normal);

	float distance = length(ex_LightDirW);
	vec3 lightDirW = normalize(ex_LightDirW);

	ambientLight = vec4(ambLightIntensity*ambLightColor.xyz, ambLightColor.w);

	// old equation, hard to tie to radius
	//float attenuation = 1.0f / (cAttenuation + lAttenuation*distance + qAttenuation*distance*distance);

	// radius based attenuation
	float attenuation = pow(max(0.0f, 1.0f - (distance/radius)), falloff+1.0f);

	float NdL = clamp(dot(normal, lightDirW), 0.0f, 1.0f);
	diffuseLight = vec4(diffuseLightIntensity*diffuseLightColor.xyz*NdL*attenuation, diffuseLightColor.w);

	if(NdL > 0.0f) {
		vec3 H = normalize(lightDirW + viewDirW);
		float NdH = clamp(dot(normal, H), 0.0f, 1.0f);
		specularLight = vec4(specularLightIntensity*diffuseLightColor.xyz*pow(NdH, specularHardness), diffuseLightColor.w);
	}

	if (ambientTexEnabled >= 1) {
        vec4 ambTextureColor = texture(texAmb,ex_UV);
		ambientLight *= ambTextureColor;
	}

	if (diffuseTexEnabled >= 1) {
		vec4 diffTextureColor = texture(texDiff,ex_UV);
		diffuseLight *= diffTextureColor;
	}
	
	out_Color = vec4(ex_Color, 1.0f)*(clamp((diffuseLight+specularLight) + ambientLight, 0.0f, 1.0f));
}