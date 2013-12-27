// Vertex Shader – file "mesh_pointlight.vert"
 
#version 140

uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;
 
in  vec3 in_Position;
in  vec3 in_Normal;
in  vec2 in_UV;

out vec2 ex_UV;
out vec3 ex_Normal;
out vec2 ex_Depth;

void main(void)
{
	ex_Normal = (in_Model * vec4(in_Normal,0)).xyz;
	ex_UV = in_UV;

	vec4 position = in_Proj * (in_View * (in_Model * vec4(in_Position,1))); 
	ex_Depth = vec2(position.z, position.w);
	gl_Position = position;
}