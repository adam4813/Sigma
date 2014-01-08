// Fragment Shader for GBuffer output
 
#version 140
 
precision highp float; // needed only for version 1.30

// Texture vars
uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform int diffuseTexEnabled;
uniform int ambientTexEnabled;
uniform float specularHardness;

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
		out_Color = vec4(texture(texDiff,ex_UV).rgb, 1.0);
	} else {
		out_Color = vec4(1.0, 1.0, 1.0, 1.0);
	}
	
	vec3 normal = normalize(ex_Normal);

	// Output normal, Adjusted to the [0.0f, 1.0f] domain
	out_Normal.rgb = (0.5 * normal) + 0.5;
	out_Normal.a = specularHardness / 1000.0;

	// Output depth, stored as one 32-bit value
	out_Depth = ex_Depth.x / ex_Depth.y;
}