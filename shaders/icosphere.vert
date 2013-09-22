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
 
void main(void)
{
	gl_Position = in_Proj * (in_View * (in_Model * vec4(in_Position,1)));
    ex_Color = in_Color;
	ex_Normal = (in_Model * vec4(in_Normal,0)).xyz;
}