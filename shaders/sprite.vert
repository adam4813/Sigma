// Vertex Shader – file "minimal.vert"
 
#version 140
 
uniform  vec2 in_Offset;
uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;

in  vec3 in_Position;
in  vec3 in_Color;
in  vec2 in_UV;
out vec2 ex_UV;
out vec3 ex_Color;
 
void main(void)
{
	vec4 v = vec4(in_Position,1);
	gl_Position = in_Model * v;
	ex_Color = in_Color;
	ex_UV = in_UV;
}