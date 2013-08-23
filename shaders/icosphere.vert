// Vertex Shader – file "minimal.vert"
 
#version 140
 
in  vec3 in_Position;
in  vec3 in_Color;
in  vec3 in_Normal;
uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;
out vec3 ex_Color;
out vec3 ex_Normal;
out vec3 ex_Light;
 
void main(void)
{
	vec4 v = vec4(in_Position,1);
	gl_Position = in_Proj * (in_View * (in_Model * v));
    ex_Color = in_Color;
	vec4 n = vec4(in_Normal,0);
	ex_Normal = (in_View * (in_Model * n)).xyz;
	vec3 light = vec3(1.0, 1.0, 1.0);
	light = normalize(light);
	vec4 l = vec4(light, 0);
	ex_Light = (in_View * l).xyz;
}