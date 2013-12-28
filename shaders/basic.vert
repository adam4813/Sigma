#version 140

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
	ex_Color = vec3(in_Color);
	ex_UV = in_UV;

  gl_Position = in_Proj * (in_View * (in_Model * vec4(in_Position, 1.0)));
}
