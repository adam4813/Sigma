// Fragment Shader for GBuffer output
 
#version 140
 
precision highp float; // needed only for version 1.30

// Texture vars
uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform int diffuseTexEnabled;
uniform int ambientTexEnabled;

in  vec3 ex_Color;
in  vec3 ex_Normal;
in  vec2 ex_UV;
in	vec2 ex_Depth;

out vec4 out_Color;
out vec4 out_Normal;
out float out_Depth;
 
void main(void)
{
	// Albedo color
	if (diffuseTexEnabled >= 1) {
		out_Color = texture(texDiff,ex_UV);
	} else {
		out_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	vec3 normal = normalize(ex_Normal);

	// Output normal, Adjusted to the [0.0f, 1.0f] domain
	out_Normal.rgb = (0.5f * normal) + 0.5f;
	out_Normal.a = 0.0f; // reserved

	// Output depth, stored as one 32-bit value
	out_Depth = ex_Depth.x / ex_Depth.y;
}