// Fragment shader - file "skybox.frag"

#version 140

uniform samplerCube cubeMap;

in vec3 ex_UVW;

out vec4 ex_Color;

void main(void) {
	ex_Color = vec4(texture(cubeMap, ex_UVW.stp).rgb, 1.0f);
}
