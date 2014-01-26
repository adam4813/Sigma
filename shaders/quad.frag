#version 330 core

in vec2 ex_UV;

out vec4 out_Color;
 
uniform sampler2D texDiff;
 
void main(void){
    out_Color = texture(texDiff, ex_UV);
}