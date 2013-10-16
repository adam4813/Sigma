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
out vec3 ex_LightDir;
 
void main(void)
{
	vec3 normalDirection = normalize(mat3(in_Model) * in_Normal);
	ex_LightDir = normalize(vec3(-in_View[3].xyz * mat3(in_View) - (in_Model * vec4(in_Position, 1.0)).xyz ));
	ex_Color = vec3(in_Color);
	ex_Normal = (in_Model * vec4(in_Normal,0)).xyz;
	ex_UV = in_UV;

	gl_Position = in_Proj * (in_View * (in_Model * vec4(in_Position,1)));
}