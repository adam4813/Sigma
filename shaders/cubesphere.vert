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
out vec3 ex_UVW;
 
void main(void)
{
	vec3 transformedPos = normalize(in_Position);
	gl_Position = in_Proj * (in_View * (in_Model * vec4(transformedPos,1)));
    ex_Color = in_Color;
	ex_Normal = (in_Model * vec4(transformedPos,0)).xyz;
	ex_UVW = transformedPos;
}