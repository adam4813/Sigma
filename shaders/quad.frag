#version 330 core

in vec2 ex_UV;

out vec4 out_Color;
 
uniform sampler2D in_Texture;
 
void main(void){
    out_Color = texture(in_Texture, ex_UV);
}