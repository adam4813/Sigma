// Fragment Shader – file "minimal.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30

uniform vec3 light = normalize(vec3(-1.0, 1.0, -1.0));
uniform samplerCube cubeMap;

in  vec3 ex_Color;
in  vec3 ex_Normal;
in  vec3 ex_Light;
in  vec3 ex_UVW;
out vec4 out_Color;
 
void main(void)
{
		float cosTheta = max( dot(normalize(ex_Normal), light), 0.0);
        //out_Color = vec4(ex_Color*cosTheta,1.0f);
		out_Color = vec4(texture(cubeMap, normalize(ex_UVW).stp).rgb*cosTheta, 1.0f);
}