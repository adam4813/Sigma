// Vertex Shader – file "skybox.vert"

// Renders an infinitely distant skybox
 
#version 140

uniform vec3 gViewPositionW;

in vec3 in_Position;

out vec4 ex_Position;
//out vec3 ex_ViewDirectionW;
out vec3 ex_UVW;

void main(void) {
	vec4 positionW = (in_Model * vec4(in_Position, 1.0f));
	//ex_ViewDirectionW = positionW.xyz - gViewPositionW;
	ex_Position = (in_Proj * (in_View * positionW)).xyww;
	ex_UVW = in_Position;
}