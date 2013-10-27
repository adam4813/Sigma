// Vertex Shader – file "icosphere.vert"
 
#version 140

uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;
 
in  vec3 in_Position;
in  vec3 in_Color;
in  vec3 in_Normal;
in  vec2 in_UV;

out vec2 ex_UV;
out vec3 ex_Color;
out vec3 ex_Normal;
out vec3 ex_PositionW;
 
void main(void)
{
	ex_Color = vec3(in_Color);
	ex_Normal = (in_Model * vec4(in_Normal,0)).xyz;
	ex_UV = in_UV;

	vec4 position = (in_Model * vec4(in_Position,1)); 
	ex_PositionW = position.xyz;
	gl_Position = in_Proj * (in_View * position);
}