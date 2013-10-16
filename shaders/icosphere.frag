// Fragment Shader – file "minimal.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30

uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform int diffuseTexEnabled;
uniform int ambientTexEnabled;
uniform vec4 diffuseLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float diffuseLightIntensity = 1.0f;
uniform vec4 ambLightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float ambLightIntensity = 0.15f;

in  vec3 ex_Color;
in  vec3 ex_Normal;
in  vec2 ex_UV;
in  vec3 ex_LightDir;
out vec4 out_Color;
 
void main(void)
{
	vec4 final_Color;
	vec4 ambientLight;

	float NdL = clamp(dot(normalize(ex_Normal), normalize(ex_LightDir)), 0.0f, 1.0f);

	if (ambientTexEnabled >= 1) {
        vec4 ambTextureColor = texture(texAmb,ex_UV);
		ambientLight = ambLightIntensity*ambLightColor*ambTextureColor;
	} else {
        ambientLight = ambLightIntensity*ambLightColor;
	}
	
	if (diffuseTexEnabled >= 1) {
		vec4 diffColor = texture(texDiff,ex_UV);
		final_Color = vec4(ex_Color, 1.0f)*diffColor*(clamp(NdL + ambientLight, 0.0f, 1.0f));
	}
	else {
		final_Color = vec4(ex_Color, 1.0f)*(clamp(NdL+ambientLight, 0.0f, 1.0f));
	}

	out_Color = final_Color;
}