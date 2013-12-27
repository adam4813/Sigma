// Vertex Shader – file "skybox.vert"

// Renders an infinitely distant skybox
 
#version 140

uniform vec3 gViewPositionW;

uniform mat4 in_Model;
uniform mat4 in_View;
uniform mat4 in_Proj;

in vec3 in_Position;

out vec3 ex_UVW;

void main(void) {
	gl_Position = (in_Proj * (in_View * (in_Model * vec4(in_Position,1)))).xyww;
	ex_UVW = in_Position;
}