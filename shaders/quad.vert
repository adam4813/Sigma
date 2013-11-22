#version 330 core

in vec3 in_Position;
in vec2 in_UV;
  
out vec2 ex_UV;

void main(void){
    gl_Position = vec4(in_Position.x, in_Position.y, 0, 1);
	ex_UV = in_UV;
}