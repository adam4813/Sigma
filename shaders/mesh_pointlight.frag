// Fragment Shader – file "minimal.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30

// Light vars
uniform vec3 lightPositionW = vec3(0.0f, 1.5f, 0.0f);
uniform vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float lightIntensity = 1.0f;
uniform float cAttenuation = 1.0f;
uniform float lAttenuation = 0.08f;
uniform float qAttenuation = 0.01f;

// Viewer position
uniform vec3 eyePositionW;

// Texture vars
uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform int diffuseTexEnabled;
uniform int ambientTexEnabled;
uniform vec4 ambLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float ambLightIntensity = 0.15f;

in  vec3 ex_Color;
in  vec3 ex_Normal;
in  vec2 ex_UV;
in  vec3 ex_PositionW;

out vec4 out_Color;
 
void main(void)
{
	vec4 diffuseLight;
	vec4 ambientLight;
	vec3 lightDir = lightPositionW - ex_PositionW;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);

	ambientLight = ambLightIntensity*ambLightColor;

	float NdL = clamp(dot(normalize(ex_Normal), lightDir), 0.0f, 1.0f);
	float attenuation = 1.0f / (cAttenuation + lAttenuation*distance + qAttenuation*distance*distance);
	diffuseLight = vec4(lightIntensity*lightColor.xyz*NdL*attenuation, lightColor.w);

	if (ambientTexEnabled >= 1) {
        vec4 ambTextureColor = texture(texAmb,ex_UV);
		ambientLight *= ambTextureColor;
	}

	if (diffuseTexEnabled >= 1) {
		vec4 diffTextureColor = texture(texDiff,ex_UV);
		diffuseLight *= diffTextureColor;
	}
	
	out_Color = vec4(ex_Color, 1.0f)*(clamp(diffuseLight+ambientLight, 0.0f, 1.0f));
}