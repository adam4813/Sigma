// Fragment shader - file "skybox.frag"

uniform samplerCube cubeMap;

in vec3 ex_UVW;

out vec4 ex_Color;

void main(void) {
	ex_Color = texture(cubeMap, ex_UVW.stp);
}
