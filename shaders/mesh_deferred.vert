// Vertex Shader – file "mesh_pointlight.vert"
 
#version 140

uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;

// Viewer position
uniform vec3 viewPosW;
uniform vec3 lightPositionW = vec3(0.0f, 1.5f, 0.0f);
 
in  vec3 in_Position;
in  vec3 in_Color;
in  vec3 in_Normal;
in  vec2 in_UV;

out vec2 ex_UV;
out vec3 ex_Color;
out vec3 ex_Normal;
out vec2 ex_Depth;
out vec3 ex_LightDirW;
out vec3 ex_ViewDirW;
 
void main(void)
{
	ex_Color = vec3(in_Color);
	ex_Normal = (in_Model * vec4(in_Normal,0)).xyz;
	ex_UV = in_UV;

	vec4 position = (in_Model * vec4(in_Position,1)); 
	ex_LightDirW = lightPositionW - position.xyz;
	ex_ViewDirW = viewPosW - position.xyz;
	posH = in_Proj * (in_View * position);
	ex_Depth = vec2(posH.z, posH.w);
	gl_Position = posH;
}