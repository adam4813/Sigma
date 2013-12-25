// Vertex Shader – file "minimal.vert"
 
#version 140
 
in  vec3 in_Position;
//in  vec3 in_Color;
//in  vec3 in_Normal;

uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;

out vec3 ex_NormalW;
out vec3 ex_TangentW;
out vec3 ex_BiNormalW;
out vec3 ex_UVW;
 
void main(void)
{
	vec3 transformedPos = normalize(in_Position);
	gl_Position = in_Proj * (in_View * (in_Model * vec4(transformedPos,1)));
	ex_NormalW = normalize((in_Model * vec4(transformedPos,0)).xyz);
	
	float x = 1 - 2*transformedPos.s;
	float y = 1 - 2*transformedPos.t;
	
	ex_TangentW = vec3(-1*(1 + pow(y, 2.0)), x*y, x);
	ex_TangentW = normalize((in_Model * vec4(ex_TangentW,0)).xyz);
	ex_BiNormalW = normalize(cross(ex_TangentW, ex_NormalW));
	ex_UVW = transformedPos;
}