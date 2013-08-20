// Vertex Shader – file "minimal.vert"
 
#version 140
 
in  vec3 in_Position;
in  vec3 in_Color;
uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;
out vec3 ex_Color;
 
void main(void)
{
	vec4 v = vec4(in_Position,1);
	gl_Position = in_Proj * (in_View * ( in_Model * v));
        ex_Color = in_Color;
}