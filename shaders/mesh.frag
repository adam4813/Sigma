// Fragment Shader – file "minimal.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30

uniform sampler2D texDiff;
uniform sampler2D texAmb;
uniform vec3 ambient_light;
uniform int texEnabled;

in  vec3 ex_Color;
in  vec3 ex_Normal;
in  vec2 ex_UV;
out vec4 out_Color;
 
void main(void)
{
	if (texEnabled >= 1) {
        	vec4 diffColor = texture(texDiff,ex_UV) * vec4(ex_Color / 2.0f,1.0f);
        	vec4 ambColor = texture(texAmb,ex_UV) * vec4(ex_Color / 2.0f,1.0f);
			out_Color = ambColor + diffColor;
	} else {
        	out_Color = vec4(ex_Color,1.0f);
	}
}